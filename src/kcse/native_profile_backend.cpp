#include "kcse/native_profile_backend.hpp"

#include "multiplayer/avatar_visual.hpp"
#include "npc/equipment_catalog.hpp"

#include <entitymodule/C_Actor.h>
#include <entitymodule/C_EquipmentManager.h>
#include <entitymodule/C_EquippableItemRuntimeData.h>
#include <entitymodule/C_Human.h>
#include <entitymodule/C_Inventory.h>
#include <entitymodule/C_Item.h>
#include <entitymodule/C_ItemDatabase.h>
#include <entitymodule/E_ItemType.h>
#include <entitymodule/S_ItemClass.h>
#include <framework/GuidUtils.h>
#include <rpgmodule/C_Soul.h>

#include <algorithm>
#include <cmath>
#include <limits>
#include <ranges>
#include <unordered_map>
#include <vector>

namespace kcd2mp::kcse
{
	namespace
	{
		constexpr std::uint32_t item_equipped = 1U;
		constexpr std::uint32_t item_change_attributes = 0x4000U;
		constexpr std::int64_t money_units_per_groschen = 10;

		protocol::AvatarWeaponClass protocol_weapon(npc::weapon_class value)
		{
			switch (value)
			{
			case npc::weapon_class::one_handed:
				return protocol::AVATAR_WEAPON_CLASS_ONE_HANDED;
			case npc::weapon_class::two_handed:
				return protocol::AVATAR_WEAPON_CLASS_TWO_HANDED;
			case npc::weapon_class::polearm:
				return protocol::AVATAR_WEAPON_CLASS_POLEARM;
			case npc::weapon_class::bow:
				return protocol::AVATAR_WEAPON_CLASS_BOW;
			case npc::weapon_class::crossbow:
				return protocol::AVATAR_WEAPON_CLASS_CROSSBOW;
			case npc::weapon_class::none:
				return protocol::AVATAR_WEAPON_CLASS_NONE;
			}
			return protocol::AVATAR_WEAPON_CLASS_NONE;
		}

		const npc::equipment_definition *equipment_for(
		    const wh::entitymodule::C_Item &item)
		{
			return item.m_pClassData
			    ? npc::runtime_equipment_catalog().find(
			        wh::FormatGuid(item.m_pClassData->m_guid))
			    : nullptr;
		}
	}

	native_profile_backend::native_profile_backend(
	    native_entity_backend &entities) :
	    m_entities(entities)
	{
	}

	void native_profile_backend::set_wire_identity(
	    const protocol::PlayerProfile &profile)
	{
		m_wire_identity = profile;
		m_avatar_state = profile.avatar();
	}

	void native_profile_backend::reset()
	{
		m_wire_identity.reset();
		m_avatar_state.reset();
	}

	std::optional<native_profile_backend::native_state>
	native_profile_backend::state(std::string &error) const
	{
		const auto local = m_entities.player();
		auto *soul = local.actor ? local.actor->m_pSoul : nullptr;
		auto *inventory =
		    soul ? soul->m_inventorySoul.GetInventory() : nullptr;
		auto *equipment =
		    soul ? soul->m_inventorySoul.GetEquipmentManager() : nullptr;
		if (!local.entity || !local.actor || !soul || !inventory || !equipment)
		{
			error =
			    "native readiness chain Entity -> Actor -> Soul -> Inventory "
			    "-> Equipment is incomplete";
			return std::nullopt;
		}
		return native_state{soul, inventory};
	}

	bool native_profile_backend::ready(std::string &error) const
	{
		if (!state(error))
			return false;
		if (npc::runtime_equipment_catalog().size() == 0
		    && !npc::initialize_runtime_equipment_catalog(error))
			return false;
		return true;
	}

	wh::entitymodule::C_Item *native_profile_backend::find_item(
	    wh::entitymodule::C_Inventory &inventory,
	    std::string_view instance_id) const
	{
		const auto found = std::ranges::find_if(
		    inventory.m_items,
		    [&](const auto *item)
		    {
			    return item
			        && wh::FormatGuid(item->m_instanceGuid) == instance_id;
		    });
		return found == inventory.m_items.end() ? nullptr : *found;
	}

	std::optional<protocol::PlayerProfile>
	native_profile_backend::capture(std::string &error)
	{
		if (!m_wire_identity)
		{
			error = "server profile identity has not been bound";
			return std::nullopt;
		}
		const auto native = state(error);
		if (!native)
			return std::nullopt;

		auto result = *m_wire_identity;
		result.clear_stats();
		result.clear_skills();
		result.clear_inventory();

		for (std::size_t index = 0; index < canonical_stat_ids.size(); ++index)
		{
			auto *value = result.add_stats();
			value->set_id(canonical_stat_ids[index]);
			value->set_level(static_cast<std::int32_t>(
			    native->soul->GetStatLevel(static_cast<std::uint32_t>(index))));
			value->set_progress(native->soul->GetStatProgress(
			    static_cast<std::uint32_t>(index)));
		}
		for (std::size_t index = 0; index < canonical_skill_ids.size(); ++index)
		{
			auto *value = result.add_skills();
			value->set_id(canonical_skill_ids[index]);
			value->set_level(static_cast<std::int32_t>(
			    native->soul->GetSkillLevel(static_cast<std::uint32_t>(index))));
			value->set_progress(native->soul->GetSkillProgress(
			    static_cast<std::uint32_t>(index)));
		}

		std::int64_t native_money{};
		for (const auto *item : native->inventory->m_items)
		{
			if (!item || !item->m_pClassData || item->m_amount <= 0)
				continue;
			if (item->IsOfType(wh::entitymodule::E_ItemType::Money))
			{
				native_money += item->m_amount;
				continue;
			}
			auto *wire = result.add_inventory();
			wire->set_instance_id(wh::FormatGuid(item->m_instanceGuid));
			wire->set_definition_id(
			    wh::FormatGuid(item->m_pClassData->m_guid));
			wire->set_count(static_cast<std::uint32_t>(item->m_amount));
			wire->set_quality(static_cast<float>(item->GetQuality()));
			wire->set_condition(item->GetCondition());
			if ((item->m_flags & item_equipped) != 0)
			{
				const auto *definition = equipment_for(*item);
				if (!definition)
				{
					error =
					    "equipped native item is absent from equipment catalog: "
					    + wire->definition_id();
					return std::nullopt;
				}
				wire->set_equipped_slot(definition->equipped_slot);
			}
		}
		if (native_money % money_units_per_groschen != 0)
		{
			error =
			    "native money contains fractional groschen that protocol v4 "
			    "cannot represent";
			return std::nullopt;
		}
		result.set_money(native_money / money_units_per_groschen);

		auto avatar = m_avatar_state.value_or(result.avatar());
		avatar.clear_equipment();
		avatar.set_weapon_class(protocol::AVATAR_WEAPON_CLASS_NONE);
		for (const auto &item : result.inventory())
		{
			if (!item.has_equipped_slot())
				continue;
			auto *visible = avatar.add_equipment();
			visible->set_definition_id(item.definition_id());
			visible->set_equipped_slot(item.equipped_slot());
			if (const auto *definition =
			        npc::runtime_equipment_catalog().find(item.definition_id());
			    definition && definition->weapon != npc::weapon_class::none)
				avatar.set_weapon_class(protocol_weapon(definition->weapon));
		}
		if (avatar.weapon_class() == protocol::AVATAR_WEAPON_CLASS_NONE)
			avatar.set_weapon_drawn(false);
		else if (const auto local = m_entities.player(); local.actor)
		{
			const auto *human =
			    reinterpret_cast<const wh::entitymodule::C_Human *>(
			        local.actor);
			avatar.set_weapon_drawn(human->IsWeaponDrawn());
			avatar.set_stance(
			    avatar.weapon_drawn()
			        ? protocol::AVATAR_STANCE_READY
			        : protocol::AVATAR_STANCE_RELAXED);
		}
		canonicalize_avatar_visual(avatar);
		*result.mutable_avatar() = std::move(avatar);

		if (const auto transform =
		        m_entities.read_transform(m_entities.player().entity))
		{
			result.set_transform_valid(true);
			*result.mutable_last_transform() = *transform;
		}
		else
		{
			result.set_transform_valid(false);
			result.clear_last_transform();
		}
		error.clear();
		return result;
	}

	bool native_profile_backend::validate_definition(
	    std::string_view definition_id,
	    std::string &error)
	{
		CryGUID guid{};
		auto *database = wh::entitymodule::C_ItemDatabase::GetInstance();
		if (!database || !wh::ParseGuid(std::string(definition_id).c_str(), guid)
		    || !database->FindClassByGuid(guid))
		{
			error = "unknown native item definition: "
			    + std::string(definition_id);
			return false;
		}
		return true;
	}

	int native_profile_backend::slot_layer(std::string_view slot) const
	{
		int result{};
		for (const auto &definition :
		     npc::runtime_equipment_catalog().entries())
			if (definition.equipped_slot == slot)
				result = std::max(result, definition.layer);
		return result;
	}

	bool native_profile_backend::unequip(
	    std::string_view instance_id,
	    std::string &error)
	{
		const auto native = state(error);
		if (!native)
			return false;
		auto *item = find_item(*native->inventory, instance_id);
		if (!item)
		{
			error = "native item to unequip does not exist";
			return false;
		}
		if ((item->m_flags & item_equipped) != 0)
			native->soul->m_inventorySoul.UnequipItem(item, true);
		if ((item->m_flags & item_equipped) != 0)
		{
			error = "native UnequipItem did not clear equipped state";
			return false;
		}
		return true;
	}

	bool native_profile_backend::remove_item(
	    std::string_view instance_id,
	    std::string &error)
	{
		const auto native = state(error);
		if (!native)
			return false;
		auto *item = find_item(*native->inventory, instance_id);
		if (!item)
		{
			error = "native item to remove does not exist";
			return false;
		}
		if ((item->m_flags & item_equipped) != 0
		    && !unequip(instance_id, error))
			return false;
		native->inventory->RemoveItem(
		    item,
		    2,
		    static_cast<std::uint32_t>(item->m_amount));
		if (find_item(*native->inventory, instance_id))
		{
			error = "native RemoveItem left the instance in inventory";
			return false;
		}
		return true;
	}

	bool native_profile_backend::create_item(
	    const protocol::InventoryItem &item,
	    std::string &error)
	{
		const auto native = state(error);
		if (!native || !validate_definition(item.definition_id(), error))
			return false;
		if (find_item(*native->inventory, item.instance_id()))
		{
			error = "native inventory already contains item instance";
			return false;
		}
		CryGUID definition{};
		CryGUID instance{};
		if (!wh::ParseGuid(item.definition_id().c_str(), definition)
		    || !wh::ParseGuid(item.instance_id().c_str(), instance))
		{
			error = "item definition or instance UUID could not be parsed";
			return false;
		}
		auto *created = native->inventory->CreateItem(
		    definition,
		    item.condition(),
		    item.count());
		if (!created)
		{
			error = "native inventory item creation failed";
			return false;
		}
		created->SetInstanceGuid(instance);
		if (!find_item(*native->inventory, item.instance_id()))
		{
			error = "native item instance GUID registration failed";
			return false;
		}
		return true;
	}

	bool native_profile_backend::update_item(
	    const protocol::InventoryItem &item,
	    std::string &error)
	{
		const auto native = state(error);
		if (!native)
			return false;
		auto *existing = find_item(*native->inventory, item.instance_id());
		if (!existing || !existing->m_pClassData
		    || wh::FormatGuid(existing->m_pClassData->m_guid)
		        != item.definition_id())
		{
			error = "native item instance/definition mismatch";
			return false;
		}
		const auto desired_count = static_cast<std::int32_t>(item.count());
		if (existing->m_amount != desired_count
		    && !native->inventory->ChangeItemAmount(
		        existing,
		        desired_count - existing->m_amount))
		{
			error = "native stack amount mutation was rejected";
			return false;
		}
		existing->SetItemHealth(item.condition());
		if (existing->IsOfType(wh::entitymodule::E_ItemType::Equippable))
		{
			auto *runtime = static_cast<
			    wh::entitymodule::C_EquippableItemRuntimeData *>(
			    existing->GetOrCreateRuntimeData());
			if (!runtime)
			{
				error = "equippable item has no runtime data";
				return false;
			}
			runtime->m_quality =
			    static_cast<std::int32_t>(std::lround(item.quality()));
			runtime->m_condition = item.condition();
			existing->NotifyChanged(item_change_attributes);
		}
		return existing->m_amount == desired_count
		    && std::abs(existing->GetCondition() - item.condition()) <= 0.001F
		    && std::abs(
		           static_cast<float>(existing->GetQuality())
		           - item.quality())
		        <= 0.01F;
	}

	bool native_profile_backend::set_money(
	    std::int64_t money,
	    std::string &error)
	{
		const auto native = state(error);
		if (!native || money < 0 || money > max_profile_money)
			return false;
		std::vector<wh::entitymodule::C_Item *> stacks;
		for (auto *item : native->inventory->m_items)
			if (item && item->IsOfType(wh::entitymodule::E_ItemType::Money))
				stacks.push_back(item);

		CryGUID money_guid{};
		if (stacks.empty())
		{
			auto *database = wh::entitymodule::C_ItemDatabase::GetInstance();
			const auto found = database
			    ? std::ranges::find_if(
			        database->m_guidIndex,
			        [](const auto &entry)
			        {
				        return entry.second
				            && entry.second->IsType(
				                wh::entitymodule::E_ItemType::Money);
			        })
			    : decltype(database->m_guidIndex.begin()){};
			if (!database || found == database->m_guidIndex.end())
			{
				error = "native money item definition is unavailable";
				return false;
			}
			money_guid = found->first;
		}

		std::int64_t remaining = money * money_units_per_groschen;
		for (auto *stack : stacks)
		{
			const auto amount = static_cast<std::int32_t>(std::min<std::int64_t>(
			    remaining,
			    std::numeric_limits<std::int32_t>::max()));
			if (amount == 0)
			{
				native->inventory->RemoveItem(
				    stack,
				    2,
				    static_cast<std::uint32_t>(stack->m_amount));
			}
			else if (const auto delta = amount - stack->m_amount;
			         delta != 0
			         && !native->inventory->ChangeItemAmount(stack, delta))
			{
				error = "native money stack mutation was rejected";
				return false;
			}
			remaining -= amount;
		}
		while (remaining > 0)
		{
			const auto amount = static_cast<std::uint32_t>(
			    std::min<std::int64_t>(
			        remaining,
			        std::numeric_limits<std::int32_t>::max()));
			auto *created =
			    native->inventory->CreateItem(money_guid, 1.0F, amount);
			if (!created)
			{
				error = "native money item creation failed";
				return false;
			}
			remaining -= amount;
		}
		return true;
	}

	bool native_profile_backend::set_rpg_value(
	    bool skill,
	    const protocol::RpgValue &value,
	    std::string &error)
	{
		const auto native = state(error);
		std::uint32_t id{};
		if (skill)
		{
			const auto found =
			    std::ranges::find(canonical_skill_ids, value.id());
			if (found == canonical_skill_ids.end())
			{
				error = "unknown canonical RPG value: " + value.id();
				return false;
			}
			id = static_cast<std::uint32_t>(
			    std::distance(canonical_skill_ids.begin(), found));
		}
		else
		{
			const auto found =
			    std::ranges::find(canonical_stat_ids, value.id());
			if (found == canonical_stat_ids.end())
			{
				error = "unknown canonical RPG value: " + value.id();
				return false;
			}
			id = static_cast<std::uint32_t>(
			    std::distance(canonical_stat_ids.begin(), found));
		}
		if (!native)
		{
			return false;
		}
		const auto applied = skill
		    ? native->soul->SetSkillAbsolute(
		        id,
		        static_cast<std::uint32_t>(value.level()),
		        value.progress())
		    : native->soul->SetStatAbsolute(
		        id,
		        static_cast<std::uint32_t>(value.level()),
		        value.progress());
		if (!applied)
			error = "native absolute RPG setter rejected " + value.id();
		return applied;
	}

	bool native_profile_backend::equip(
	    std::string_view instance_id,
	    std::string_view slot,
	    std::string &error)
	{
		const auto native = state(error);
		if (!native)
			return false;
		auto *item = find_item(*native->inventory, instance_id);
		const auto *definition = item ? equipment_for(*item) : nullptr;
		if (!item || !definition || definition->equipped_slot != slot)
		{
			error = "native item cannot be equipped in requested slot";
			return false;
		}
		native->soul->m_inventorySoul.EquipItem(item, true);
		if ((item->m_flags & item_equipped) == 0)
		{
			error = "native EquipItem did not set equipped state";
			return false;
		}
		return true;
	}

	bool native_profile_backend::set_avatar_state(
	    const protocol::AvatarDescriptor &avatar,
	    std::string &error)
	{
		if (!is_valid_avatar_descriptor(avatar))
		{
			error = "target avatar descriptor is invalid";
			return false;
		}
		const auto local = m_entities.player();
		if (!local.actor)
		{
			error = "native local Human is unavailable";
			return false;
		}
		auto *human =
		    reinterpret_cast<wh::entitymodule::C_Human *>(local.actor);
		const auto should_draw =
		    avatar.weapon_class() != protocol::AVATAR_WEAPON_CLASS_NONE
		    && (avatar.weapon_drawn()
		        || avatar.stance() == protocol::AVATAR_STANCE_READY);
		if (human->IsWeaponDrawn() != should_draw
		    && !human->SetWeaponDrawn(should_draw))
		{
			error = should_draw
			    ? "native Human rejected weapon draw"
			    : "native Human rejected weapon holster";
			return false;
		}
		m_avatar_state = avatar;
		m_avatar_state->set_weapon_drawn(should_draw);
		m_avatar_state->set_stance(
		    should_draw ? protocol::AVATAR_STANCE_READY
		                : protocol::AVATAR_STANCE_RELAXED);
		return true;
	}

	bool native_profile_backend::set_transform(
	    const protocol::TransformState &transform,
	    std::string &error)
	{
		return m_entities.write_transform(
		    m_entities.player().entity,
		    transform,
		    error);
	}
}
