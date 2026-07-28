#pragma once

#include "multiplayer/protocol.hpp"

#include <algorithm>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace kcd2mp
{
	inline void canonicalize_avatar_visual(
	    protocol::AvatarDescriptor &avatar)
	{
		std::vector<protocol::AvatarEquipment> equipment{
		    avatar.equipment().begin(),
		    avatar.equipment().end()};
		std::ranges::sort(
		    equipment,
		    {},
		    [](const protocol::AvatarEquipment &item)
		    {
			    return std::pair{
			        item.equipped_slot(),
			        item.definition_id()};
		    });
		if (equipment.size() > max_avatar_equipment_items)
			equipment.resize(max_avatar_equipment_items);
		avatar.clear_equipment();
		for (auto &item : equipment)
			*avatar.add_equipment() = std::move(item);
	}

	[[nodiscard]] inline bool same_avatar_visual(
	    const protocol::AvatarDescriptor &left,
	    const protocol::AvatarDescriptor &right)
	{
		if (left.archetype_id() != right.archetype_id()
		    || left.stance() != right.stance()
		    || left.weapon_class() != right.weapon_class()
		    || left.weapon_drawn() != right.weapon_drawn()
		    || left.equipment_size() != right.equipment_size())
		{
			return false;
		}
		for (int index = 0; index < left.equipment_size(); ++index)
		{
			if (left.equipment(index).definition_id()
			        != right.equipment(index).definition_id()
			    || left.equipment(index).equipped_slot()
			        != right.equipment(index).equipped_slot())
			{
				return false;
			}
		}
		return true;
	}

	[[nodiscard]] inline protocol::AvatarDescriptor merge_avatar_visual(
	    const protocol::AvatarDescriptor &authoritative,
	    const std::optional<protocol::AvatarDescriptor> &local_visual,
	    const std::optional<std::string> &selected_archetype)
	{
		auto result = authoritative;
		if (selected_archetype)
			result.set_archetype_id(*selected_archetype);
		if (local_visual)
		{
			result.clear_equipment();
			for (const auto &item : local_visual->equipment())
				*result.add_equipment() = item;
			result.set_stance(local_visual->stance());
			result.set_weapon_class(local_visual->weapon_class());
			result.set_weapon_drawn(local_visual->weapon_drawn());
		}
		result.set_revision(authoritative.revision());
		canonicalize_avatar_visual(result);
		return result;
	}
}
