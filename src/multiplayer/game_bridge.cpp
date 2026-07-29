#include "multiplayer/game_bridge.hpp"

#include "kcd2_init.hpp"
#include "kcse/bridge_client.hpp"
#include "multiplayer/avatar_visual.hpp"
#include "multiplayer/client.hpp"
#include "multiplayer/entity_control.hpp"
#include "multiplayer/remote_avatar.hpp"
#include "npc/catalog.hpp"
#include "npc/equipment_catalog.hpp"
#include "npc/xgen_lifecycle.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <format>
#include <lua/lua_manager.hpp>
#include <string_view>
#include <unordered_map>

namespace kcd2mp::game
{
	namespace
	{
		using clock = std::chrono::steady_clock;

		std::uint64_t sequence{};
		big::Vec3 previous_position{};
		clock::time_point previous_sample{};
		bool previous_position_valid{};

		class retail_entity_control_backend final : public entity_control_backend
		{
		public:
			bool should_disable(controlled_entity entity) const override
			{
				// Inventory, HUD/Flash, cameras, particles, lights, equipment,
				// and other client-side helpers are CEntities too. Hiding every
				// non-player CEntity corrupts the inventory render pass and
				// destroys transient equipment such as the player's torch.
				// ENTITY_FLAG_HAS_AI is the engine-owned boundary for world
				// actors that this server control is intended to isolate.
				return (static_cast<big::CEntity *>(entity)->GetFlags()
				        & big::ENTITY_FLAG_HAS_AI)
				    != 0;
			}

			bool is_active(controlled_entity entity) const override
			{
				return static_cast<big::CEntity *>(entity)->IsActive();
			}

			bool is_hidden(controlled_entity entity) const override
			{
				return static_cast<big::CEntity *>(entity)->IsHidden();
			}

			bool set_active(controlled_entity entity, bool active) override
			{
				static_cast<big::CEntity *>(entity)->Activate(active);
				return true;
			}

			bool set_hidden(controlled_entity entity, bool hidden) override
			{
				static_cast<big::CEntity *>(entity)->Hide(hidden);
				return true;
			}
		};

		retail_entity_control_backend entity_backend;
		entity_controller entities{entity_backend};

		struct lua_call_result
		{
			bool success{};
			sol::object value;
			std::string error;
		};

		template<typename... Args>
		lua_call_result call_lua(const sol::protected_function &function, Args &&...args)
		{
			if (!function.valid())
			{
				return {false, {}, "Lua function is unavailable"};
			}
			auto result = function(std::forward<Args>(args)...);
			if (!result.valid())
			{
				const sol::error error = result;
				return {false, {}, error.what()};
			}
			return {true, result.return_count() == 0 ? sol::object{} : result.get<sol::object>(), {}};
		}

		template<typename... Args>
		lua_call_result call_lua_method(const sol::table &table, std::string_view name, Args &&...args)
		{
			// KCD2 exposes engine-backed instance methods through the table's
			// metatable (__index), so a raw lookup incorrectly reports methods
			// such as Entity.EnablePhysics as unavailable.
			const sol::object member = table.get<sol::object>(name);
			if (member.get_type() != sol::type::function)
			{
				return {false, {}, std::format("Lua method '{}' is unavailable", name)};
			}
			return call_lua(member.as<sol::protected_function>(), table, std::forward<Args>(args)...);
		}

		std::optional<sol::table> lua_table_member(const sol::table &table, std::string_view name)
		{
			const sol::object member = table.raw_get<sol::object>(name);
			return member.get_type() == sol::type::table ? std::optional(member.as<sol::table>()) : std::nullopt;
		}

		std::optional<std::string> lua_object_string(sol::state_view lua, const sol::object &value)
		{
			if (value.get_type() == sol::type::string)
			{
				return value.as<std::string>();
			}
			const sol::object tostring_object = lua.globals().raw_get<sol::object>("tostring");
			if (tostring_object.get_type() != sol::type::function)
			{
				return std::nullopt;
			}
			const auto converted = call_lua(tostring_object.as<sol::protected_function>(), value);
			return converted.success && converted.value.get_type() == sol::type::string ? std::optional(converted.value.as<std::string>()) : std::nullopt;
		}

		sol::table lua_vec3(sol::state_view lua, float x, float y, float z)
		{
			auto result = lua.create_table();
			result["x"] = x;
			result["y"] = y;
			result["z"] = z;
			return result;
		}

		void matrix_from_npc_transform(const npc::transform &transform, float (&matrix)[12])
		{
			auto [x, y, z, w] = transform.rotation;
			const auto length = std::sqrt(x * x + y * y + z * z + w * w);
			if (length <= 0.000001F)
			{
				x = y = z = 0.0F;
				w         = 1.0F;
			}
			else
			{
				x /= length;
				y /= length;
				z /= length;
				w /= length;
			}
			matrix[0]  = 1.0F - 2.0F * (y * y + z * z);
			matrix[1]  = 2.0F * (x * y - z * w);
			matrix[2]  = 2.0F * (x * z + y * w);
			matrix[3]  = transform.position[0];
			matrix[4]  = 2.0F * (x * y + z * w);
			matrix[5]  = 1.0F - 2.0F * (x * x + z * z);
			matrix[6]  = 2.0F * (y * z - x * w);
			matrix[7]  = transform.position[1];
			matrix[8]  = 2.0F * (x * z - y * w);
			matrix[9]  = 2.0F * (y * z + x * w);
			matrix[10] = 1.0F - 2.0F * (x * x + y * y);
			matrix[11] = transform.position[2];
		}

		class retail_npc_backend final : public npc::backend
		{
		public:
			npc::capability get_capability() const override
			{
				const auto kcse_status = kcse::current_bridge_status();
				if (!kcse_status.available)
				{
					return {false, kcse_status.diagnostic};
				}
				if (!big::g_lua_manager || !big::g_lua_manager->lua_state())
				{
					return {false, "the KCD2 Game Lua VM is not initialized"};
				}
				if (npc::runtime_equipment_catalog().size() == 0)
				{
					return {false, "the active KCD2 equipment catalog could not be loaded"};
				}
				sol::state_view lua(big::g_lua_manager->lua_state());
				const auto globals                  = lua.globals();
				const auto required_global_function = [&](std::string_view table_name, std::string_view function_name)
				{
					const auto table = lua_table_member(globals, table_name);
					return table && table->raw_get<sol::object>(function_name).get_type() == sol::type::function;
				};
				const auto xgen = lua_table_member(globals, "XGenAIModule");
				const auto spawn_binding =
				    npc::select_lua_spawn_binding(xgen && xgen->raw_get<sol::object>("SpawnEntity").get_type() == sol::type::function, required_global_function("System", "SpawnEntity"));
				if (spawn_binding == npc::lua_spawn_binding::unavailable)
				{
					return {false,
					        "required KCD2 Lua spawn binding "
					        "XGenAIModule.SpawnEntity or "
					        "System.SpawnEntity is unavailable"};
				}
				if (spawn_binding == npc::lua_spawn_binding::system
				    && !required_global_function(
				        "AI",
				        "SetBehaviorTreeEvaluationEnabled"))
				{
					return {
					    false,
					    "required KCD2 Lua binding "
					    "AI.SetBehaviorTreeEvaluationEnabled is unavailable"};
				}
				for (const auto &[table, function] : std::array{std::pair{std::string_view{"System"}, std::string_view{"GetEntity"}}, std::pair{std::string_view{"System"}, std::string_view{"GetEntityByName"}}, std::pair{std::string_view{"System"}, std::string_view{"RemoveEntity"}}, std::pair{std::string_view{"ItemManager"}, std::string_view{"GetItem"}}, std::pair{std::string_view{"EntityModule"}, std::string_view{"GetSlotItemClassId"}}})
				{
					if (!required_global_function(table, function))
					{
						return {false, std::format("required KCD2 Lua binding {}.{} is unavailable", table, function)};
					}
				}
				if (!big::g_player_entity)
				{
					return {false, "the local KCD2 player entity is unavailable"};
				}
				kcse::entity_view native_player;
				if (!kcse::resolve_entity(big::g_player_entity->GetId(), native_player)
				    || !native_player.actor || !native_player.soul)
				{
					return {
					    false,
					    "KCSE/libKCD2 could not resolve the local Actor/Soul pair"};
				}
				const auto system = *lua_table_member(globals, "System");
				const auto player = call_lua(system.raw_get<sol::protected_function>("GetEntity"), big::g_player_entity->GetId());
				if (!player.success || player.value.get_type() != sol::type::table)
				{
					return {false, "the local KCD2 player script entity is unavailable"};
				}
				const auto entity = player.value.as<sol::table>();
				for (const auto component : {"actor", "human", "inventory", "soul"})
				{
					if (!lua_table_member(entity, component))
					{
						return {false, std::format("local KCD2 player has no '{}' component", component)};
					}
				}
				const auto actor     = *lua_table_member(entity, "actor");
				const auto human     = *lua_table_member(entity, "human");
				const auto inventory = *lua_table_member(entity, "inventory");
				const std::array required_methods{std::pair{&entity, "EnablePhysics"}, std::pair{&actor, "EquipInventoryItem"}, std::pair{&human, "DrawWeapon"}, std::pair{&human, "HolsterWeapon"}, std::pair{&human, "IsWeaponDrawn"}, std::pair{&inventory, "CreateItem"}, std::pair{&inventory, "FindItem"}, std::pair{&inventory, "DeleteItem"}, std::pair{&inventory, "GetInventoryTable"}};
				std::string missing_methods;
				for (const auto &[table, method] : required_methods)
				{
					if (table->get<sol::object>(method).get_type() != sol::type::function)
					{
						if (!missing_methods.empty())
						{
							missing_methods += ", ";
						}
						missing_methods += method;
					}
				}
				if (!missing_methods.empty())
				{
					return {false, "required KCD2 Lua methods are unavailable: " + missing_methods};
				}
				return {
				    true,
				    spawn_binding == npc::lua_spawn_binding::xgen_ai_module
				        ? "KCSE/libKCD2 Actor/Soul bridge and KCD2 XGen lifecycle are ready"
				        : "KCSE/libKCD2 Actor/Soul bridge and KCD2 System lifecycle fallback are ready"};
			}

			std::optional<npc::native_handle> spawn(const npc::spawn_request &request, std::string &error) override
			{
				const auto capability = get_capability();
				if (!capability.available)
				{
					error = capability.diagnostic;
					return std::nullopt;
				}
				sol::state_view lua(big::g_lua_manager->lua_state());
				const auto globals = lua.globals();
				const auto xgen    = lua_table_member(globals, "XGenAIModule");
				const auto system  = *lua_table_member(globals, "System");
				const auto spawn_binding =
				    npc::select_lua_spawn_binding(xgen && xgen->raw_get<sol::object>("SpawnEntity").get_type() == sol::type::function,
				                                  system.raw_get<sol::object>("SpawnEntity").get_type() == sol::type::function);
				const auto handle           = m_next_handle++;
				const auto spawn_parameters = npc::make_xgen_spawn_parameters(request, handle);
				lua_call_result spawned;
				std::string_view spawn_function;
				if (spawn_binding == npc::lua_spawn_binding::xgen_ai_module)
				{
					auto parameters              = lua.create_table();
					parameters["Name"]           = spawn_parameters.name;
					parameters["ClassName"]      = spawn_parameters.class_name;
					parameters["SharedSoulGuid"] = spawn_parameters.shared_soul_guid;
					parameters["Pos"] =
					    lua_vec3(lua, spawn_parameters.position[0], spawn_parameters.position[1], spawn_parameters.position[2]);
					parameters["Rot"] =
					    lua_vec3(lua, spawn_parameters.rotation[0], spawn_parameters.rotation[1], spawn_parameters.rotation[2]);
					parameters["NoAI"]                = spawn_parameters.no_ai;
					parameters["IdleUntilFirstPatch"] = spawn_parameters.idle_until_first_patch;
					parameters["PerceptorObjectAI"]   = spawn_parameters.perceptor_object_ai;
					parameters["PerceptibleObjectAI"] = spawn_parameters.perceptible_object_ai;
					spawned        = call_lua(xgen->raw_get<sol::protected_function>("SpawnEntity"), parameters);
					spawn_function = "XGenAIModule.SpawnEntity";
				}
				else
				{
					auto parameters      = lua.create_table();
					parameters["name"]   = spawn_parameters.name;
					const auto archetype = npc::runtime_catalog().find(request.archetype_id);
					parameters["class"]  = archetype ? archetype->archetype_name : spawn_parameters.class_name;
					parameters["position"] =
					    lua_vec3(lua, spawn_parameters.position[0], spawn_parameters.position[1], spawn_parameters.position[2]);
					auto properties                            = lua.create_table();
					properties["guidSharedSoulId"]             = spawn_parameters.shared_soul_guid;
					properties["sharedSoulGuid"]               = spawn_parameters.shared_soul_guid;
					properties["bWH_PerceptorObject"]          = spawn_parameters.perceptor_object_ai;
					properties["bWH_PerceptibleObject"]        = spawn_parameters.perceptible_object_ai;
					properties["bWH_ListenerObject"]           = false;
					properties["bWH_CreateSituationSubsystem"] = false;
					properties["bWH_RequiresHome"]             = false;
					parameters["properties"]                   = properties;
					spawned        = call_lua(system.raw_get<sol::protected_function>("SpawnEntity"), parameters);
					spawn_function = "System.SpawnEntity";
				}
				if (!spawned.success)
				{
					error = std::format("{} failed: {}", spawn_function, spawned.error);
					return std::nullopt;
				}
				runtime_npc runtime;
				runtime.diagnostic_context = request.diagnostic_context.empty() ? "controlled NPC" : request.diagnostic_context;
				runtime.entity_name                = spawn_parameters.name;
				runtime.exempt_from_entity_control = request.exempt_from_entity_control;
				runtime.disable_behavior_tree =
				    spawn_binding == npc::lua_spawn_binding::system;
				if (spawned.value.get_type() == sol::type::table)
				{
					runtime.script = spawned.value.as<sol::table>();
					runtime.entity_id =
					    runtime.script.raw_get<sol::object>("id");
				}
				else if (spawned.value.valid()
				         && spawned.value.get_type() != sol::type::nil)
				{
					runtime.entity_id = spawned.value;
				}
				m_npcs.emplace(handle, std::move(runtime));
				return handle;
			}

			npc::status poll(npc::native_handle handle) override
			{
				const auto found = m_npcs.find(handle);
				if (found == m_npcs.end())
				{
					return {npc::state::failed, npc::error_code::externally_destroyed, "KCD2 NPC runtime handle is missing"};
				}
				auto &runtime = found->second;
				if (!runtime.failure.empty())
				{
					return {npc::state::failed, npc::error_code::spawn_failed, runtime.failure};
				}
				if (runtime.entity)
				{
					return {npc::state::ready};
				}

				sol::state_view lua(big::g_lua_manager->lua_state());
				const auto system = *lua_table_member(lua.globals(), "System");
				if (!runtime.script.valid()
				    || !runtime.entity_id.valid()
				    || runtime.entity_id.get_type() == sol::type::nil)
				{
					lua_call_result entity_result;
					if (!runtime.script.valid()
					    && runtime.entity_id.valid()
					    && runtime.entity_id.get_type() != sol::type::nil)
					{
						entity_result = call_lua(
						    system.raw_get<sol::protected_function>(
						        "GetEntity"),
						    runtime.entity_id);
					}
					if (!entity_result.success
					    || entity_result.value.get_type()
					           != sol::type::table)
					{
						entity_result = call_lua(
						    system.raw_get<sol::protected_function>(
						        "GetEntityByName"),
						    runtime.entity_name);
					}
					if (!entity_result.success)
					{
						runtime.failure = entity_result.error;
						return {
						    npc::state::failed,
						    npc::error_code::spawn_failed,
						    runtime.failure};
					}
					if (entity_result.value.get_type()
					    != sol::type::table)
					{
						return {npc::state::pending};
					}
					runtime.script =
					    entity_result.value.as<sol::table>();
					runtime.entity_id =
					    runtime.script.raw_get<sol::object>("id");
				}
				if (!runtime.entity_id.valid()
				    || runtime.entity_id.get_type() == sol::type::nil)
				{
					return {npc::state::pending};
				}
				const auto entity    = runtime.script;
				const auto actor     = lua_table_member(entity, "actor");
				const auto human     = lua_table_member(entity, "human");
				const auto inventory = lua_table_member(entity, "inventory");
				const auto soul      = lua_table_member(entity, "soul");
				if (!actor || !human || !inventory || !soul)
				{
					return {npc::state::pending};
				}

				const auto native = std::ranges::find_if(big::g_entities,
				                                         [&](big::CEntity *candidate)
				                                         {
					                                         return candidate
					                                             && candidate->GetName()
					                                             && runtime.entity_name
					                                                    == candidate->GetName();
				                                         });
				if (native == big::g_entities.end())
				{
					return {npc::state::pending};
				}
				kcse::entity_view native_view;
				if (!kcse::resolve_entity((*native)->GetId(), native_view)
				    || !native_view.actor || !native_view.soul)
				{
					return {npc::state::pending};
				}

				runtime.entity     = *native;
				runtime.native_actor = native_view.actor;
				runtime.native_soul = native_view.soul;
				runtime.script     = entity;
				runtime.actor      = *actor;
				runtime.human      = *human;
				runtime.inventory  = *inventory;
				if (runtime.disable_behavior_tree)
				{
					const auto ai =
					    *lua_table_member(lua.globals(), "AI");
					const auto disabled = call_lua(
					    ai.raw_get<sol::protected_function>(
					        "SetBehaviorTreeEvaluationEnabled"),
					    runtime.entity_id,
					    false);
					if (!disabled.success)
					{
						runtime.failure =
						    "could not disable remote NPC behavior tree: "
						    + disabled.error;
						return {
						    npc::state::failed,
						    npc::error_code::spawn_failed,
						    runtime.failure};
					}
				}
				const auto physics = call_lua_method(runtime.script, "EnablePhysics", false);
				if (!physics.success)
				{
					runtime.failure = "could not disable remote NPC physics: " + physics.error;
					return {npc::state::failed, npc::error_code::spawn_failed, runtime.failure};
				}
				if (runtime.exempt_from_entity_control)
				{
					register_player_entity(runtime.entity);
					runtime.registered = true;
				}
				return {npc::state::ready};
			}

			bool set_transform(npc::native_handle handle, const npc::transform &value) override
			{
				const auto found = m_npcs.find(handle);
				if (found == m_npcs.end() || !found->second.entity)
				{
					return false;
				}
				float matrix[12];
				matrix_from_npc_transform(value, matrix);
				found->second.entity->SetWorldTM(matrix);
				found->second.last_transform = value;
				return true;
			}

			bool set_motion(npc::native_handle handle, const npc::motion &value) override
			{
				const auto found = m_npcs.find(handle);
				if (found == m_npcs.end() || !found->second.entity)
				{
					return false;
				}
				auto &runtime        = found->second;
				if (runtime.actor.get<sol::object>("SetMovementTarget").get_type() != sol::type::function)
				{
					// KCD2 retail removed this legacy Actor binding. World
					// transforms still provide authoritative remote movement;
					// only locomotion animation is unavailable on that build.
					return true;
				}
				auto velocity        = value.velocity;
				float speed          = std::sqrt(velocity[0] * velocity[0] + velocity[1] * velocity[1]);
				const auto &rotation = runtime.last_transform.rotation;
				std::array forward{2.0F * (rotation[0] * rotation[1] - rotation[2] * rotation[3]), 1.0F - 2.0F * (rotation[0] * rotation[0] + rotation[2] * rotation[2]), 0.0F};
				if (speed > 0.001F)
				{
					forward = {velocity[0] / speed, velocity[1] / speed, 0.0F};
				}
				else
				{
					speed = value.mode == npc::locomotion::walk ? 1.8F :
					        value.mode == npc::locomotion::run  ? 4.5F :
					                                              0.0F;
				}
				sol::state_view lua(big::g_lua_manager->lua_state());
				const auto &position = runtime.last_transform.position;
				const auto movement = call_lua_method(runtime.actor, "SetMovementTarget", lua_vec3(lua, position[0], position[1], position[2]), lua_vec3(lua, position[0] + forward[0], position[1] + forward[1], position[2]), lua_vec3(lua, 0.0F, 0.0F, 1.0F), speed);
				return movement.success;
			}

			bool set_appearance(npc::native_handle handle, const npc::appearance &value) override
			{
				const auto found = m_npcs.find(handle);
				if (found == m_npcs.end() || !found->second.entity)
				{
					return false;
				}
				auto &runtime = found->second;
				std::vector<const npc::equipment_definition *> desired;
				desired.reserve(value.items.size());
				for (const auto &item : value.items)
				{
					const auto *definition = npc::runtime_equipment_catalog().find(item.definition_id);
					if (!definition || definition->equipped_slot != item.equipped_slot)
					{
						LOGF(WARNING,
						     "Skipping remote avatar item {} in slot {} for {}: definition is unavailable or "
						     "mismatched.",
						     item.definition_id,
						     item.equipped_slot,
						     runtime.diagnostic_context);
						continue;
					}
					desired.push_back(definition);
				}
				std::ranges::sort(desired,
				                  [](const auto *left, const auto *right)
				                  {
					                  if (left->layer != right->layer)
					                  {
						                  return left->layer < right->layer;
					                  }
					                  return left->equipped_slot < right->equipped_slot;
				                  });

				std::vector<sol::object> created;
				const auto discard_created = [&]
				{
					for (const auto &item : created)
					{
						(void)call_lua_method(runtime.inventory, "DeleteItem", item, 1);
					}
				};
				const auto restore_previous = [&]
				{
					for (const auto &item : runtime.created_items)
					{
						(void)call_lua_method(runtime.actor, "EquipInventoryItem", item);
					}
				};
				for (const auto *definition : desired)
				{
					const auto created_item =
					    call_lua_method(runtime.inventory, "CreateItem", definition->definition_id, 1.0F, 1);
					if (!created_item.success
					    || (created_item.value.get_type() == sol::type::boolean
					        && !created_item.value.as<bool>()))
					{
						const auto diagnostic =
						    created_item.error.empty() ? "returned false" : created_item.error;
						LOGF(WARNING,
						     "Skipping remote avatar item {} for {}: CreateItem failed ({}).",
						     definition->definition_id,
						     runtime.diagnostic_context,
						     diagnostic);
						continue;
					}
					const auto added = call_lua_method(runtime.inventory, "FindItem", definition->definition_id);
					if (!added.success || added.value.get_type() == sol::type::nil || added.value.get_type() == sol::type::none)
					{
						LOGF(WARNING,
						     "Skipping remote avatar item {} for {}: FindItem could not resolve the created item ({}).",
						     definition->definition_id,
						     runtime.diagnostic_context,
						     added.error);
						continue;
					}
					const auto equipped = call_lua_method(runtime.actor, "EquipInventoryItem", added.value);
					if (!equipped.success)
					{
						(void)call_lua_method(runtime.inventory, "DeleteItem", added.value, 1);
						LOGF(WARNING,
						     "Skipping remote avatar item {} for {}: EquipInventoryItem failed ({}).",
						     definition->definition_id,
						     runtime.diagnostic_context,
						     equipped.error);
						continue;
					}
					created.push_back(added.value);
				}

				if (value.weapon_drawn)
				{
					const int weapon_set = value.weapon == npc::weapon_class::bow || value.weapon == npc::weapon_class::crossbow ? 1 : 0;
					if (runtime.human.get<sol::object>("ToggleWeaponSet").get_type() == sol::type::function)
					{
						const auto toggle = call_lua_method(runtime.human, "ToggleWeaponSet", weapon_set);
						if (!toggle.success)
						{
							restore_previous();
							discard_created();
							return false;
						}
					}
				}
				const auto weapon = call_lua_method(runtime.human, value.weapon_drawn ? "DrawWeapon" : "HolsterWeapon");
				if (!weapon.success)
				{
					restore_previous();
					discard_created();
					return false;
				}

				for (const auto &old_item : runtime.created_items)
				{
					(void)call_lua_method(runtime.inventory, "DeleteItem", old_item, 1);
				}
				runtime.created_items = std::move(created);
				return true;
			}

			void remove(npc::native_handle handle) override
			{
				const auto found = m_npcs.find(handle);
				if (found == m_npcs.end())
				{
					return;
				}
				auto &runtime    = found->second;
				runtime.removing = true;
				if (runtime.registered && runtime.entity)
				{
					unregister_player_entity(runtime.entity);
				}
				if (big::g_lua_manager && big::g_lua_manager->lua_state())
				{
					sol::state_view lua(big::g_lua_manager->lua_state());
					if (const auto system = lua_table_member(lua.globals(), "System"))
					{
						auto entity_id = runtime.entity_id;
						if ((!entity_id.valid()
						     || entity_id.get_type() == sol::type::nil)
						    && runtime.entity)
						{
							entity_id = sol::make_object(
							    lua,
							    runtime.entity->GetId());
						}
						if (!entity_id.valid()
						    || entity_id.get_type() == sol::type::nil)
						{
							const auto entity = call_lua(
							    system->raw_get<sol::protected_function>(
							        "GetEntityByName"),
							    runtime.entity_name);
							if (entity.success
							    && entity.value.get_type()
							           == sol::type::table)
							{
								entity_id =
								    entity.value.as<sol::table>()
								        .raw_get<sol::object>("id");
							}
						}
						if (entity_id.valid()
						    && entity_id.get_type() != sol::type::nil)
						{
							(void)call_lua(
							    system->raw_get<sol::protected_function>(
							        "RemoveEntity"),
							    entity_id);
						}
					}
				}
				m_npcs.erase(found);
			}

			std::optional<npc::native_handle> entity_destroyed(big::CEntity *entity)
			{
				for (auto iterator = m_npcs.begin(); iterator != m_npcs.end(); ++iterator)
				{
					if (iterator->second.entity != entity)
					{
						continue;
					}
					if (iterator->second.removing)
					{
						return std::nullopt;
					}
					const auto handle = iterator->first;
					m_npcs.erase(iterator);
					return handle;
				}
				return std::nullopt;
			}

		private:
			struct runtime_npc
			{
				std::string diagnostic_context;
				std::string entity_name;
				sol::object entity_id;
				big::CEntity *entity{};
				void *native_actor{};
				void *native_soul{};
				sol::table script;
				sol::table actor;
				sol::table human;
				sol::table inventory;
				std::vector<sol::object> created_items;
				npc::transform last_transform;
				std::string failure;
				bool exempt_from_entity_control{};
				bool disable_behavior_tree{};
				bool registered{};
				bool removing{};
			};

			npc::native_handle m_next_handle{1};
			std::unordered_map<npc::native_handle, runtime_npc> m_npcs;
		};

		retail_npc_backend native_npc_backend;
		npc::manager controlled_npcs{native_npc_backend};

		std::optional<const npc::equipment_definition *> equipped_item_definition(sol::state_view lua, const sol::table &item_manager, const sol::table &entity_module, const sol::object &item_id)
		{
			const auto item_result = call_lua(item_manager.raw_get<sol::protected_function>("GetItem"), item_id);
			if (!item_result.success || item_result.value.get_type() != sol::type::table)
			{
				return std::nullopt;
			}
			const auto item = item_result.value.as<sol::table>();
			const auto slot = call_lua_method(item, "GetSlot", true);
			if (!slot.success || slot.value.get_type() == sol::type::nil || slot.value.get_type() == sol::type::none)
			{
				return std::nullopt;
			}
			const auto class_id = call_lua(entity_module.raw_get<sol::protected_function>("GetSlotItemClassId"), slot.value);
			if (!class_id.success)
			{
				return std::nullopt;
			}
			const auto text = lua_object_string(lua, class_id.value);
			if (!text || !is_uuid(*text))
			{
				return std::nullopt;
			}
			const auto *definition = npc::runtime_equipment_catalog().find(*text);
			return definition ? std::optional(definition) : std::nullopt;
		}

		protocol::AvatarWeaponClass protocol_weapon_class(npc::weapon_class value)
		{
			return static_cast<protocol::AvatarWeaponClass>(static_cast<int>(value));
		}

		std::optional<protocol::AvatarDescriptor> local_avatar_visual(clock::time_point now = clock::now())
		{
			static clock::time_point next_sample;
			static std::optional<protocol::AvatarDescriptor> cached;
			static bool failure_logged{};
			if (!big::g_player_entity || !big::g_lua_manager || !big::g_lua_manager->lua_state())
			{
				cached.reset();
				next_sample    = {};
				failure_logged = false;
				return std::nullopt;
			}
			if (now < next_sample)
			{
				return cached;
			}
			next_sample = now + std::chrono::milliseconds(250);

			sol::state_view lua(big::g_lua_manager->lua_state());
			const auto globals       = lua.globals();
			const auto system        = lua_table_member(globals, "System");
			const auto item_manager  = lua_table_member(globals, "ItemManager");
			const auto entity_module = lua_table_member(globals, "EntityModule");
			if (!system || !item_manager || !entity_module)
			{
				return cached;
			}
			const auto player_result =
			    call_lua(system->raw_get<sol::protected_function>("GetEntity"), big::g_player_entity->GetId());
			if (!player_result.success || player_result.value.get_type() != sol::type::table)
			{
				return cached;
			}
			const auto player    = player_result.value.as<sol::table>();
			const auto inventory = lua_table_member(player, "inventory");
			const auto human     = lua_table_member(player, "human");
			if (!inventory || !human)
			{
				return cached;
			}
			const auto inventory_result = call_lua_method(*inventory, "GetInventoryTable");
			if (!inventory_result.success || inventory_result.value.get_type() != sol::type::table)
			{
				if (!failure_logged)
				{
					LOG(WARNING) << "Could not capture local avatar inventory: " << inventory_result.error;
					failure_logged = true;
				}
				return cached;
			}

			protocol::AvatarDescriptor visual;
			visual.set_revision(1);
			std::unordered_map<std::string, const npc::equipment_definition *> equipped;
			for (const auto &entry : inventory_result.value.as<sol::table>())
			{
				sol::object item_id = entry.second;
				if (item_id.get_type() == sol::type::table)
				{
					const auto item_table = item_id.as<sol::table>();
					for (const auto key : {"id", "itemId", "item_id"})
					{
						const auto candidate = item_table.raw_get<sol::object>(key);
						if (candidate.valid() && candidate.get_type() != sol::type::nil)
						{
							item_id = candidate;
							break;
						}
					}
				}
				const auto definition = equipped_item_definition(lua, *item_manager, *entity_module, item_id);
				if (definition)
				{
					equipped[(*definition)->equipped_slot] = *definition;
				}
			}
			std::vector<const npc::equipment_definition *> ordered;
			ordered.reserve(equipped.size());
			for (const auto &[slot, definition] : equipped)
			{
				(void)slot;
				ordered.push_back(definition);
			}
			std::ranges::sort(ordered,
			                  [](const auto *left, const auto *right)
			                  {
				                  if (left->layer != right->layer)
				                  {
					                  return left->layer < right->layer;
				                  }
				                  return left->equipped_slot < right->equipped_slot;
			                  });
			for (const auto *definition : ordered)
			{
				auto *item = visual.add_equipment();
				item->set_definition_id(definition->definition_id);
				item->set_equipped_slot(definition->equipped_slot);
			}

			bool weapon_drawn{};
			const auto drawn = call_lua_method(*human, "IsWeaponDrawn");
			if (drawn.success && drawn.value.get_type() == sol::type::boolean)
			{
				weapon_drawn = drawn.value.as<bool>();
			}
			npc::weapon_class active_weapon{npc::weapon_class::none};
			if (weapon_drawn)
			{
				for (const int hand : {0, 1})
				{
					const auto current = call_lua_method(*human, "GetItemInHand", hand);
					if (!current.success)
					{
						continue;
					}
					const auto definition =
					    equipped_item_definition(lua, *item_manager, *entity_module, current.value);
					if (definition && (*definition)->weapon != npc::weapon_class::none)
					{
						active_weapon = (*definition)->weapon;
						break;
					}
				}
			}
			if (weapon_drawn && active_weapon == npc::weapon_class::none)
			{
				const auto weapon = std::ranges::find_if(ordered,
				                                         [](const auto *definition)
				                                         {
					                                         return definition->weapon != npc::weapon_class::none;
				                                         });
				if (weapon != ordered.end())
				{
					active_weapon = (*weapon)->weapon;
				}
			}
			weapon_drawn = weapon_drawn && active_weapon != npc::weapon_class::none;
			visual.set_weapon_drawn(weapon_drawn);
			visual.set_weapon_class(protocol_weapon_class(active_weapon));
			visual.set_stance(weapon_drawn ? protocol::AVATAR_STANCE_READY : protocol::AVATAR_STANCE_RELAXED);
			canonicalize_avatar_visual(visual);
			cached         = std::move(visual);
			failure_logged = false;
			return cached;
		}

		std::uint64_t pack_npc_handle(npc::handle value)
		{
			return static_cast<std::uint64_t>(value.generation) << 32 | value.slot;
		}

		npc::handle unpack_npc_handle(std::uint64_t value)
		{
			return {static_cast<std::uint32_t>(value), static_cast<std::uint32_t>(value >> 32)};
		}

		npc::transform npc_transform(const protocol::TransformState &value)
		{
			return {{value.position().x(), value.position().y(), value.position().z()},
			        {value.rotation().x(), value.rotation().y(), value.rotation().z(), value.rotation().w()}};
		}

		npc::appearance npc_appearance(const protocol::AvatarDescriptor &value)
		{
			npc::appearance result;
			result.items.reserve(value.equipment_size());
			for (const auto &item : value.equipment())
			{
				result.items.push_back({item.definition_id(), item.equipped_slot()});
			}
			result.pose   = value.stance() == protocol::AVATAR_STANCE_READY ? npc::stance::ready : npc::stance::relaxed;
			result.weapon = static_cast<npc::weapon_class>(static_cast<int>(value.weapon_class()));
			result.weapon_drawn = value.weapon_drawn();
			return result;
		}

		npc::locomotion npc_locomotion(protocol::MovementMode value)
		{
			switch (value)
			{
			case protocol::MOVEMENT_MODE_WALK: return npc::locomotion::walk;
			case protocol::MOVEMENT_MODE_RUN:  return npc::locomotion::run;
			default:                           return npc::locomotion::idle;
			}
		}

		class retail_remote_avatar_backend final : public remote_avatar_backend
		{
		public:
			bool available() const override
			{
				return controlled_npcs.get_capability().available;
			}

			std::string diagnostic() const override
			{
				return controlled_npcs.get_capability().diagnostic;
			}

			std::optional<remote_avatar_handle> spawn(const remote_avatar_snapshot &player) override
			{
				npc::spawn_request request;
				request.diagnostic_context         = std::format("player {}", player.id);
				request.archetype_id               = player.avatar.archetype_id();
				request.world_transform            = npc_transform(player.transform);
				request.movement                   = npc_locomotion(player.movement_mode);
				request.velocity                   = {player.transform.velocity().x(),
				                                      player.transform.velocity().y(),
				                                      player.transform.velocity().z()};
				request.visual                     = npc_appearance(player.avatar);
				request.exempt_from_entity_control = true;
				const auto spawned                 = controlled_npcs.spawn(std::move(request), player.id);
				return spawned ? std::optional<remote_avatar_handle>{pack_npc_handle(spawned.npc)} : std::nullopt;
			}

			bool update(remote_avatar_handle avatar, const remote_avatar_snapshot &player, bool appearance_changed) override
			{
				const auto handle = unpack_npc_handle(avatar);
				const auto status = controlled_npcs.get_status(handle);
				if (status.value == npc::state::failed || status.value == npc::state::removed)
				{
					return false;
				}
				const bool dynamic_applied = controlled_npcs.set_transform(handle, npc_transform(player.transform))
				                             && controlled_npcs.set_motion(handle,
				                                                           npc::motion{npc_locomotion(player.movement_mode),
				                                                                       {player.transform.velocity().x(),
				                                                                        player.transform.velocity().y(),
				                                                                        player.transform.velocity().z()}});
				return dynamic_applied
				       && (!appearance_changed || controlled_npcs.set_appearance(handle, npc_appearance(player.avatar)));
			}

			void remove(remote_avatar_handle avatar) override
			{
				(void)controlled_npcs.remove(unpack_npc_handle(avatar));
			}

			remote_avatar_backend_status status(remote_avatar_handle avatar) const override
			{
				const auto current = controlled_npcs.get_status(unpack_npc_handle(avatar));
				switch (current.value)
				{
				case npc::state::ready:   return {remote_avatar_state::ready};
				case npc::state::failed:
				case npc::state::removed: return {remote_avatar_state::failed, current.diagnostic};
				default:                  return {remote_avatar_state::pending};
				}
			}
		};

		retail_remote_avatar_backend remote_avatar_backend;
		remote_avatar_manager remote_avatars{remote_avatar_backend};

		struct cvar_change
		{
			std::string_view name;
			int previous{};
		};

		struct sandbox_runtime
		{
			sandbox_phase phase{sandbox_phase::idle};
			std::string expected_level_id;
			std::string map_name;
			std::string error;
			std::optional<protocol::TransformState> initial_spawn;
			std::optional<protocol::ServerBootstrap> bootstrap;
			std::vector<cvar_change> cvar_changes;
			clock::time_point deadline;
			std::optional<clock::time_point> player_ready_since;
			bool player_observed{};
		};

		sandbox_runtime sandbox;

		constexpr std::array level_names{
		    std::pair{"0", "rataje"},
		    std::pair{"1", "rataje_dlc4"},
		    std::pair{"2", "trosecko"},
		    std::pair{"3", "kutnohorsko"},
		    std::pair{"4", "klaster"},
		    std::pair{"256", "test_switching256"},
		    std::pair{"257", "test_switching257"},
		    std::pair{"258", "concept_level_switch_1"},
		    std::pair{"259", "concept_level_switch_2"},
		    std::pair{"300", "empty"},
		    std::pair{"400", "test_save"},
		    std::pair{"401", "test_switch_first"},
		    std::pair{"402", "test_switch_second"},
		    std::pair{"500", "player_switching"},
		    std::pair{"501", "player_switching2"},
		};

		constexpr std::array required_cvars{
		    std::pair{"g_EnableLoadSave", 0},
		    std::pair{"autotest_disable_saveload", 1},
		    // The retail map path identifies a frontend map load as a new game
		    // and otherwise plays intro_new_game. This Warhorse-owned switch is
		    // registered without VF_CHEAT in the supported retail image.
		    std::pair{"wh_sys_HideIntroVideo", 1},
		    // Do not disable all movie sequences here: the native level-start
		    // sequence also dismisses the loading UI and activates the player.
		    std::pair{"g_disableSequencePlayback", 0},
		    std::pair{"wh_sys_FreezePlayline", 1},
		    std::pair{"wh_sys_NoPlaylineDeleting", 1},
		    std::pair{"wh_sys_AutoLoadLastSave", 0},
		    std::pair{"g_asynclevelload", 0},
		};

		std::optional<std::string_view> map_name_for_level(std::string_view level_id)
		{
			const auto found = std::ranges::find(level_names, level_id, &decltype(level_names)::value_type::first);
			return found == level_names.end() ? std::nullopt : std::optional<std::string_view>(found->second);
		}

		void restore_cvars()
		{
			for (auto change = sandbox.cvar_changes.rbegin(); change != sandbox.cvar_changes.rend(); ++change)
			{
				if (!big::engine_cvar_set_int(change->name, change->previous))
				{
					LOGF(ERROR, "Sandbox cleanup could not restore {}={}.", change->name, change->previous);
				}
			}
			sandbox.cvar_changes.clear();
		}

		void reset_sandbox_runtime()
		{
			sandbox                 = {};
			previous_position_valid = false;
		}

		bool apply_sandbox_cvars(std::string &error)
		{
			for (const auto &[name, value] : required_cvars)
			{
				const auto previous = big::engine_cvar_int(name);
				if (!previous)
				{
					error = std::format("required sandbox CVar '{}' is unavailable", name);
					restore_cvars();
					return false;
				}
				sandbox.cvar_changes.push_back({name, *previous});
				if (!big::engine_cvar_set_int(name, value))
				{
					error = std::format("sandbox CVar '{}' rejected value {}", name, value);
					restore_cvars();
					return false;
				}
			}
			return true;
		}

		protocol::Quaternion quaternion_from_matrix(const float (&matrix)[12])
		{
			protocol::Quaternion result;
			const auto trace = matrix[0] + matrix[5] + matrix[10];
			if (trace > 0.0F)
			{
				const auto scale = std::sqrt(trace + 1.0F) * 2.0F;
				result.set_w(0.25F * scale);
				result.set_x((matrix[9] - matrix[6]) / scale);
				result.set_y((matrix[2] - matrix[8]) / scale);
				result.set_z((matrix[4] - matrix[1]) / scale);
			}
			else if (matrix[0] > matrix[5] && matrix[0] > matrix[10])
			{
				const auto scale = std::sqrt(1.0F + matrix[0] - matrix[5] - matrix[10]) * 2.0F;
				result.set_w((matrix[9] - matrix[6]) / scale);
				result.set_x(0.25F * scale);
				result.set_y((matrix[1] + matrix[4]) / scale);
				result.set_z((matrix[2] + matrix[8]) / scale);
			}
			else if (matrix[5] > matrix[10])
			{
				const auto scale = std::sqrt(1.0F + matrix[5] - matrix[0] - matrix[10]) * 2.0F;
				result.set_w((matrix[2] - matrix[8]) / scale);
				result.set_x((matrix[1] + matrix[4]) / scale);
				result.set_y(0.25F * scale);
				result.set_z((matrix[6] + matrix[9]) / scale);
			}
			else
			{
				const auto scale = std::sqrt(1.0F + matrix[10] - matrix[0] - matrix[5]) * 2.0F;
				result.set_w((matrix[4] - matrix[1]) / scale);
				result.set_x((matrix[2] + matrix[8]) / scale);
				result.set_y((matrix[6] + matrix[9]) / scale);
				result.set_z(0.25F * scale);
			}
			(void)normalize_rotation(&result);
			return result;
		}

		void matrix_from_transform(const protocol::TransformState &transform, float (&matrix)[12])
		{
			auto rotation = transform.rotation();
			if (!normalize_rotation(&rotation))
			{
				rotation.Clear();
				rotation.set_w(1.0F);
			}
			const auto x = rotation.x();
			const auto y = rotation.y();
			const auto z = rotation.z();
			const auto w = rotation.w();

			matrix[0]  = 1.0F - 2.0F * (y * y + z * z);
			matrix[1]  = 2.0F * (x * y - z * w);
			matrix[2]  = 2.0F * (x * z + y * w);
			matrix[3]  = transform.position().x();
			matrix[4]  = 2.0F * (x * y + z * w);
			matrix[5]  = 1.0F - 2.0F * (x * x + z * z);
			matrix[6]  = 2.0F * (y * z - x * w);
			matrix[7]  = transform.position().y();
			matrix[8]  = 2.0F * (x * z - y * w);
			matrix[9]  = 2.0F * (y * z + x * w);
			matrix[10] = 1.0F - 2.0F * (x * x + y * y);
			matrix[11] = transform.position().z();
		}
	} // namespace

	sandbox_gate sandbox_capability()
	{
		if (!big::engine_console_available())
		{
			return {false, "Sandbox engine console is not initialized yet."};
		}
		for (const auto command : {"unload"})
		{
			if (!big::engine_console_has_command(command))
			{
				return {false, std::format("Required retail console command '{}' is unavailable.", command)};
			}
		}
		for (const auto &[name, value] : required_cvars)
		{
			(void)value;
			if (!big::engine_cvar_available(name))
			{
				return {false, std::format("Required sandbox CVar '{}' is unavailable.", name)};
			}
		}
		if (!big::engine_cvar_available("wh_sys_BaseLevelId"))
		{
			return {false, "Required level-detection CVar 'wh_sys_BaseLevelId' is unavailable."};
		}
		if (!big::g_CEntity_SetWorldTM)
		{
			return {false, "The signature-gated player transform wrapper is unavailable."};
		}
		const auto avatar = native_npc_backend.get_capability();
		if (!avatar.available)
		{
			return {false, "Remote avatar lifecycle is unavailable: " + avatar.diagnostic};
		}
		return {true, "Retail sandbox and KCD2 remote-avatar lifecycle wrappers are ready."};
	}

	bool can_start_join()
	{
		return sandbox.phase == sandbox_phase::idle && big::g_player_entity && !current_level_id().empty();
	}

	sandbox_start_result begin_sandbox(const protocol::ServerBootstrap &bootstrap)
	{
		const auto capability = sandbox_capability();
		if (!capability.available)
		{
			return {false, capability.diagnostic};
		}
		if (sandbox.phase != sandbox_phase::idle)
		{
			return {false, "another sandbox transition is already active"};
		}
		if (!big::g_player_entity)
		{
			return {false, "the native save was no longer loaded when the server bootstrap arrived"};
		}
		const auto map_name = map_name_for_level(bootstrap.level_id());
		if (!map_name)
		{
			return {false, std::format("server requested unsupported retail level id '{}'", bootstrap.level_id())};
		}
		const auto loaded_level = current_level_id();
		if (loaded_level.empty())
		{
			return {false, "the loaded save has no detectable retail level id"};
		}
		if (loaded_level != bootstrap.level_id())
		{
			return {false,
			        std::format("loaded save is on level id {}, but the server requires level id {}",
			                    loaded_level,
			                    bootstrap.level_id())};
		}

		sandbox.bootstrap         = bootstrap;
		sandbox.expected_level_id = bootstrap.level_id();
		sandbox.map_name          = *map_name;
		sandbox.error.clear();
		sandbox.initial_spawn.reset();
		sandbox.player_observed = false;
		if (!apply_sandbox_cvars(sandbox.error))
		{
			const auto error = sandbox.error;
			reset_sandbox_runtime();
			return {false, error};
		}

		sandbox.player_observed    = true;
		sandbox.player_ready_since = clock::now();
		LOGF(INFO,
		     "Sandbox bootstrap adopted the natively loaded save on server level {}; save/load is now locked.",
		     sandbox.expected_level_id);
		sandbox.phase = sandbox_phase::loading;
		const auto timeout =
		    std::max<std::uint32_t>(5, bootstrap.timeout_seconds() > 5 ? bootstrap.timeout_seconds() - 5 : bootstrap.timeout_seconds());
		sandbox.deadline = clock::now() + std::chrono::seconds(timeout);
		return {true, {}};
	}

	sandbox_poll_result poll_sandbox()
	{
		if (sandbox.phase == sandbox_phase::unloading)
		{
			sandbox.player_observed = sandbox.player_observed || big::g_player_entity != nullptr;
			if (sandbox.player_observed && !big::g_player_entity)
			{
				restore_cvars();
				LOG(INFO) << "Sandbox unloaded and engine CVars restored.";
				reset_sandbox_runtime();
			}
			else if (clock::now() >= sandbox.deadline)
			{
				sandbox.phase = sandbox_phase::failed;
				sandbox.error = "sandbox unload timed out; save/load remains locked for safety";
				LOG(ERROR) << sandbox.error;
			}
			return {sandbox.phase, sandbox.error, std::nullopt};
		}

		if (sandbox.phase != sandbox_phase::loading)
		{
			return {sandbox.phase, sandbox.error, sandbox.initial_spawn};
		}
		if (clock::now() >= sandbox.deadline)
		{
			sandbox.phase = sandbox_phase::failed;
			sandbox.error = std::format("timed out loading retail level '{}' (id {})", sandbox.map_name, sandbox.expected_level_id);
			return {sandbox.phase, sandbox.error, std::nullopt};
		}

		const auto level = current_level_id();
		if (!big::g_player_entity || level.empty())
		{
			sandbox.player_ready_since.reset();
			return {sandbox_phase::loading, {}, std::nullopt};
		}
		if (level != sandbox.expected_level_id)
		{
			sandbox.player_ready_since.reset();
			return {sandbox_phase::loading, {}, std::nullopt};
		}
		sandbox.player_observed = true;
		if (!sandbox.player_ready_since)
		{
			sandbox.player_ready_since = clock::now();
			return {sandbox_phase::loading, {}, std::nullopt};
		}
		// The Dude entity is published before the retail map transition has
		// completely settled. Requiring a stable player/level pair avoids
		// sending ClientWorldReady while the native loading UI is still closing.
		if (clock::now() - *sandbox.player_ready_since < std::chrono::seconds(5))
		{
			return {sandbox_phase::loading, {}, std::nullopt};
		}
		if (!sandbox.bootstrap)
		{
			sandbox.phase = sandbox_phase::failed;
			sandbox.error = "sandbox bootstrap state was lost while loading";
			return {sandbox.phase, sandbox.error, std::nullopt};
		}

		const auto &bootstrap                        = *sandbox.bootstrap;
		const protocol::TransformState *target_spawn = nullptr;
		if (bootstrap.has_profile() && bootstrap.profile().transform_valid() && bootstrap.profile().has_last_transform())
		{
			target_spawn = &bootstrap.profile().last_transform();
		}
		else if (bootstrap.spawn_valid() && bootstrap.has_spawn())
		{
			target_spawn = &bootstrap.spawn();
		}
		if (target_spawn)
		{
			if (!is_finite_transform(*target_spawn))
			{
				sandbox.phase = sandbox_phase::failed;
				sandbox.error = "server supplied an invalid sandbox spawn transform";
				return {sandbox.phase, sandbox.error, std::nullopt};
			}
			apply_local_correction(*target_spawn);
		}

		const auto spawned = local_transform();
		if (!spawned)
		{
			return {sandbox_phase::loading, {}, std::nullopt};
		}
		if (bootstrap.mode() == protocol::BOOTSTRAP_MODE_INITIALIZE)
		{
			sandbox.initial_spawn = *spawned;
		}
		sandbox.phase = sandbox_phase::ready;
		LOGF(INFO, "Sandbox level {} is ready and the server spawn was applied.", sandbox.expected_level_id);
		return {sandbox.phase, {}, sandbox.initial_spawn};
	}

	bool sandbox_active()
	{
		return sandbox.phase != sandbox_phase::idle;
	}

	void end_sandbox()
	{
		if (sandbox.phase == sandbox_phase::idle || sandbox.phase == sandbox_phase::unloading)
		{
			return;
		}
		sandbox.initial_spawn.reset();
		sandbox.bootstrap.reset();
		const auto removed_avatars = remote_avatars.clear();
		if (removed_avatars != 0)
		{
			LOGF(INFO, "Removed {} remote multiplayer avatars.", removed_avatars);
		}
		(void)set_non_player_entities_disabled(false);
		if (!big::g_player_entity)
		{
			restore_cvars();
			reset_sandbox_runtime();
			return;
		}
		if (!big::engine_console_execute("unload", true))
		{
			sandbox.phase = sandbox_phase::failed;
			sandbox.error = "could not queue sandbox unload; save/load remains locked for safety";
			LOG(ERROR) << sandbox.error;
			return;
		}
		sandbox.phase    = sandbox_phase::unloading;
		sandbox.deadline = clock::now() + std::chrono::seconds(30);
		LOG(INFO) << "Sandbox unload queued.";
	}

	std::string current_level_id()
	{
		if (!big::g_player_entity)
		{
			return {};
		}
		const auto level = big::engine_cvar_string("wh_sys_BaseLevelId");
		return level.value_or(std::string{});
	}

	std::optional<protocol::TransformState> local_transform()
	{
		if (!big::g_player_entity)
		{
			previous_position_valid = false;
			return std::nullopt;
		}

		float matrix[12];
		big::g_player_entity->GetWorldTM(matrix);
		const big::Vec3 position{matrix[3], matrix[7], matrix[11]};
		const auto now = std::chrono::steady_clock::now();

		big::Vec3 velocity{};
		if (previous_position_valid && now > previous_sample)
		{
			const auto seconds = std::chrono::duration<float>(now - previous_sample).count();
			if (seconds > 0.0F)
			{
				velocity = (position - previous_position) * (1.0F / seconds);
			}
		}
		previous_position       = position;
		previous_sample         = now;
		previous_position_valid = true;

		protocol::TransformState result;
		result.mutable_position()->set_x(position.x);
		result.mutable_position()->set_y(position.y);
		result.mutable_position()->set_z(position.z);
		*result.mutable_rotation() = quaternion_from_matrix(matrix);
		result.mutable_velocity()->set_x(velocity.x);
		result.mutable_velocity()->set_y(velocity.y);
		result.mutable_velocity()->set_z(velocity.z);
		result.set_sequence(++sequence);
		result.set_client_time_ms(
		    static_cast<std::uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count()));
		return result;
	}

	std::optional<protocol::PlayerProfile> local_profile()
	{
		// Profile capture is part of the native sandbox risk gate. Returning no
		// snapshot keeps the client from claiming it captured authoritative RPG
		// or inventory data through an unaudited fallback.
		return std::nullopt;
	}

	void apply_local_correction(const protocol::TransformState &transform)
	{
		if (!big::g_player_entity || !is_finite_transform(transform))
		{
			return;
		}
		float matrix[12];
		matrix_from_transform(transform, matrix);
		big::g_player_entity->SetWorldTM(matrix);
	}

	bool set_non_player_entities_disabled(bool disabled)
	{
		if (big::g_player_entity)
		{
			(void)entities.register_player(big::g_player_entity);
		}
		std::vector<controlled_entity> current;
		current.reserve(big::g_entities.size());
		for (auto *entity : big::g_entities)
		{
			current.push_back(entity);
		}
		const auto result = entities.set_disabled(disabled, current);
		LOGF(result.failed == 0 ? INFO : ERROR,
		     "Server entity control {}: affected={}, restored={}, failed={}.",
		     disabled ? "disabled AI entities" : "restored AI entities",
		     result.affected,
		     result.restored,
		     result.failed);
		return result.failed == 0;
	}

	bool non_player_entities_disabled()
	{
		return entities.disabled();
	}

	void register_player_entity(void *entity)
	{
		const auto result = entities.register_player(entity);
		if (result.failed != 0)
		{
			LOG(ERROR) << "Could not restore a newly registered multiplayer player entity.";
			if (g_multiplayer_client)
			{
				g_multiplayer_client->fail("could not exempt a multiplayer player entity "
				                           "from server entity control");
			}
		}
	}

	void unregister_player_entity(void *entity)
	{
		entities.unregister_player(entity);
	}

	void on_entity_created(void *entity)
	{
		const auto result = entities.entity_created(entity);
		if (result.failed != 0)
		{
			LOG(ERROR) << "Could not apply server entity control to a newly created entity.";
			if (g_multiplayer_client)
			{
				g_multiplayer_client->fail("could not apply server entity control "
				                           "to a newly created entity");
			}
		}
	}

	void on_entity_destroyed(void *entity)
	{
		entities.entity_destroyed(entity);
		if (const auto handle = native_npc_backend.entity_destroyed(static_cast<big::CEntity *>(entity)))
		{
			controlled_npcs.native_destroyed(*handle);
		}
	}

	npc::manager &npc_manager()
	{
		return controlled_npcs;
	}

	void game_thread_tick()
	{
		static const bool catalog_loaded = []
		{
			std::string error;
			if (!npc::initialize_runtime_catalog(error))
			{
				LOG(ERROR) << "NPC catalog initialization failed: " << error;
				return false;
			}
			LOGF(INFO, "Loaded {} human Soul archetypes from local Tables.pak.", npc::runtime_catalog().size());
			if (!npc::initialize_runtime_equipment_catalog(error))
			{
				LOG(ERROR) << "Equipment catalog initialization failed: " << error;
				return false;
			}
			LOGF(INFO,
			     "Loaded {} visible equipment definitions from local Tables.pak.",
			     npc::runtime_equipment_catalog().size());
			return true;
		}();
		(void)catalog_loaded;
		if (!g_multiplayer_client)
		{
			return;
		}
		(void)poll_sandbox();
		controlled_npcs.tick();
		const auto level = current_level_id();
		g_multiplayer_client->game_tick(local_transform(), local_avatar_visual(), level);
		const auto status = g_multiplayer_client->status();
		if (status.state == client_state::connected || status.state == client_state::reconnecting)
		{
			std::vector<remote_avatar_snapshot> snapshots;
			for (const auto &player : g_multiplayer_client->remote_players())
			{
				snapshots.push_back({player.id,
				                     player.display_name,
				                     player.connected,
				                     player.has_transform,
				                     player.transform,
				                     player.movement_mode,
				                     player.has_avatar,
				                     player.avatar});
			}
			const auto avatars = remote_avatars.sync(snapshots);
			if (!avatars.success)
			{
				g_multiplayer_client->fail(avatars.error);
			}
			else if (!avatars.diagnostic.empty())
			{
				LOG(WARNING) << avatars.diagnostic;
			}
		}
		else if (remote_avatars.size() != 0)
		{
			(void)remote_avatars.clear();
		}
		if (const auto correction = g_multiplayer_client->take_local_correction())
		{
			apply_local_correction(*correction);
		}
		if (g_multiplayer_client->status().state == client_state::disconnected && sandbox_active())
		{
			end_sandbox();
		}
	}
} // namespace kcd2mp::game
