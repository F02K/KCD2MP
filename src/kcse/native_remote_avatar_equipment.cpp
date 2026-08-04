#include "kcse/native_remote_avatar_equipment.hpp"

#include "kcse/join_trace.hpp"
#include "kcse/native_equipment.hpp"
#include "kcse/native_inventory.hpp"
#include "kcse/native_weapon_controller.hpp"
#include "npc/equipment_catalog.hpp"

#include <entitymodule/C_EquipmentManager.h>
#include <entitymodule/C_Human.h>
#include <entitymodule/C_Inventory.h>
#include <entitymodule/C_Item.h>
#include <entitymodule/C_ItemDatabase.h>
#include <framework/GuidUtils.h>
#include <rpgmodule/C_Soul.h>

#include <algorithm>
#include <chrono>
#include <format>
#include <random>
#include <ranges>

namespace kcd2mp::kcse
{
	namespace
	{
		struct desired_equipment_item
		{
			const protocol::AvatarEquipment *wire{};
			CryGUID guid{};
			int layer{};
		};

		CryGUID make_instance_guid()
		{
			thread_local std::mt19937_64 generator{
			    std::random_device{}()
			    ^ static_cast<std::uint64_t>(
			        std::chrono::high_resolution_clock::now()
			            .time_since_epoch()
			            .count())};
			CryGUID result{};
			static_assert(sizeof(result) == 16);
			auto *parts = reinterpret_cast<std::uint64_t *>(&result);
			parts[0] = generator();
			parts[1] = generator();
			auto *bytes = reinterpret_cast<std::uint8_t *>(&result);
			bytes[6] = static_cast<std::uint8_t>(
			    (bytes[6] & 0x0FU) | 0x40U);
			bytes[8] = static_cast<std::uint8_t>(
			    (bytes[8] & 0x3FU) | 0x80U);
			return result;
		}

		bool remove_item(
		    wh::rpgmodule::C_Soul &soul,
		    wh::entitymodule::C_Inventory &inventory,
		    wh::entitymodule::C_Item &item,
		    std::string_view instance_id,
		    std::string &error)
		{
			if ((item.m_flags & native_item_equipped) != 0)
			{
				soul.m_inventorySoul.UnequipItem(&item, true);
				if ((item.m_flags & native_item_equipped) != 0)
				{
					error = "remote native UnequipItem left the item equipped";
					return false;
				}
			}
			inventory.RemoveItem(
			    &item,
			    2,
			    static_cast<std::uint32_t>(item.m_amount));
			if (find_inventory_item(inventory, instance_id))
			{
				error = "remote item cleanup left an inventory instance";
				return false;
			}
			return true;
		}

		bool discard_rejected_item(
		    const native_remote_equipment_context &native,
		    wh::entitymodule::C_Item &item,
		    std::string_view instance_id,
		    std::string &error)
		{
			return remove_item(
			    native.soul,
			    native.inventory,
			    item,
			    instance_id,
			    error);
		}
	}

	bool clear_native_remote_equipment(
	    wh::rpgmodule::C_Soul &soul,
	    wh::entitymodule::C_Inventory &inventory,
	    std::vector<std::string> &item_instances,
	    std::string &error)
	{
		for (auto iterator = item_instances.rbegin();
		     iterator != item_instances.rend(); ++iterator)
		{
			auto *item = find_inventory_item(inventory, *iterator);
			if (item
			    && !remove_item(soul, inventory, *item, *iterator, error))
			{
				return false;
			}
		}
		item_instances.clear();
		return true;
	}

	bool apply_native_remote_equipment(
	    const native_remote_equipment_context &native,
	    const protocol::AvatarDescriptor &appearance,
	    std::vector<std::string> &item_instances,
	    std::string &error)
	{
		if (native.human.IsWeaponDrawn()
		    && !set_weapon_set_drawn(
		        native.human,
		        native_weapon_set::any,
		        false))
		{
			error = "remote equipment change could not holster active weapon";
			return false;
		}

		std::vector<desired_equipment_item> desired;
		desired.reserve(appearance.equipment_size());
		for (const auto &wire : appearance.equipment())
		{
			CryGUID guid{};
			if (!wh::ParseGuid(wire.definition_id().c_str(), guid))
			{
				error = "remote equipment definition UUID is invalid: "
				    + wire.definition_id();
				return false;
			}
			if (!native.database.FindClassByGuid(guid))
			{
				KCD2MP_JOIN_TRACE(
				    "join.remote-appearance.item-skipped",
				    std::format(
				        "player_id={} definition=\"{}\" slot=\"{}\" "
				        "reason=native-definition-unavailable",
				        native.player,
				        wire.definition_id(),
				        wire.equipped_slot()));
				continue;
			}
			desired.push_back({
			    &wire,
			    guid,
			    npc::runtime_equipment_catalog().layer_for_slot(
			        wire.equipped_slot())});
		}
		std::ranges::sort(
		    desired,
		    [](const desired_equipment_item &left,
		       const desired_equipment_item &right)
		    {
			    return left.layer < right.layer;
		    });
		if (!clear_native_remote_equipment(
		        native.soul,
		        native.inventory,
		        item_instances,
		        error))
		{
			return false;
		}

		for (const auto &item : desired)
		{
			KCD2MP_JOIN_TRACE(
			    "join.remote-appearance.CreateItem.begin",
			    std::format(
			        "player_id={} entity_id={} definition=\"{}\" "
			        "slot=\"{}\" api=fork:C_InventoryBase::CreateItem",
			        native.player,
			        native.entity_id,
			        item.wire->definition_id(),
			        item.wire->equipped_slot()));
			auto *created = native.inventory.CreateItem(item.guid, 1.0F, 1);
			KCD2MP_JOIN_TRACE(
			    created ? "join.remote-appearance.CreateItem.returned"
			            : "join.remote-appearance.CreateItem.nil",
			    std::format(
			        "player_id={} entity_id={} item={}",
			        native.player,
			        native.entity_id,
			        static_cast<void *>(created)));
			if (!created)
			{
				KCD2MP_JOIN_TRACE(
				    "join.remote-appearance.item-skipped",
				    std::format(
				        "player_id={} definition=\"{}\" slot=\"{}\" "
				        "reason=native-item-creation-failed",
				        native.player,
				        item.wire->definition_id(),
				        item.wire->equipped_slot()));
				continue;
			}

			const auto instance = make_instance_guid();
			KCD2MP_JOIN_TRACE(
			    "join.remote-appearance.SetInstanceGuid.begin",
			    std::format(
			        "player_id={} entity_id={} item={}",
			        native.player,
			        native.entity_id,
			        static_cast<void *>(created)));
			created->SetInstanceGuid(instance);
			const auto instance_text = wh::FormatGuid(instance);
			item_instances.push_back(instance_text);
			KCD2MP_JOIN_TRACE(
			    "join.remote-appearance.EquipItem.begin",
			    std::format(
			        "player_id={} entity_id={} item={} instance=\"{}\"",
			        native.player,
			        native.entity_id,
			        static_cast<void *>(created),
			        instance_text));
			native.soul.m_inventorySoul.EquipItem(created, true);
			KCD2MP_JOIN_TRACE(
			    "join.remote-appearance.EquipItem.returned",
			    std::format(
			        "player_id={} entity_id={} item_flags=0x{:08X}",
			        native.player,
			        native.entity_id,
			        created->m_flags));

			if ((created->m_flags & native_item_equipped) == 0)
			{
				KCD2MP_JOIN_TRACE(
				    "join.remote-appearance.item-skipped",
				    std::format(
				        "player_id={} definition=\"{}\" slot=\"{}\" "
				        "reason=native-equip-rejected",
				        native.player,
				        item.wire->definition_id(),
				        item.wire->equipped_slot()));
				if (!discard_rejected_item(
				        native,
				        *created,
				        instance_text,
				        error))
				{
					return false;
				}
				item_instances.pop_back();
				continue;
			}

			auto *equipment =
			    native.soul.m_inventorySoul.GetEquipmentManager();
			const auto actual_slot = equipment
			    ? native_equipped_slot(*equipment, *created)
			    : std::nullopt;
			if (actual_slot
			    && *actual_slot == item.wire->equipped_slot())
			{
				continue;
			}

			KCD2MP_JOIN_TRACE(
			    "join.remote-appearance.item-skipped",
			    std::format(
			        "player_id={} definition=\"{}\" requested_slot=\"{}\" "
			        "actual_slot=\"{}\" reason=native-slot-mismatch",
			        native.player,
			        item.wire->definition_id(),
			        item.wire->equipped_slot(),
			        actual_slot.value_or("")));
			if (!discard_rejected_item(
			        native,
			        *created,
			        instance_text,
			        error))
			{
				return false;
			}
			item_instances.pop_back();
		}
		return true;
	}
}
