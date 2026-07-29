#include "multiplayer/avatar_visual.hpp"

#include <cassert>

int main()
{
	using namespace kcd2mp;
	protocol::AvatarDescriptor authoritative;
	authoritative.set_archetype_id(
	    "763db0bb-4469-497d-bdc9-712b3df91b5a");
	authoritative.set_revision(7);

	protocol::AvatarDescriptor local;
	local.set_stance(protocol::AVATAR_STANCE_READY);
	local.set_weapon_class(
	    protocol::AVATAR_WEAPON_CLASS_ONE_HANDED);
	local.set_weapon_drawn(true);
	auto *right = local.add_equipment();
	right->set_definition_id("aaaaaaaa-aaaa-4aaa-8aaa-aaaaaaaaaaaa");
	right->set_equipped_slot("PrimaryMainHand");
	auto *body = local.add_equipment();
	body->set_definition_id("bbbbbbbb-bbbb-4bbb-8bbb-bbbbbbbbbbbb");
	body->set_equipped_slot("body_cloth_padded");

	const auto merged = merge_avatar_visual(
	    authoritative,
	    local,
	    std::string{"11111111-2222-4333-8444-555555555555"});
	assert(merged.revision() == 7);
	assert(merged.equipment_size() == 2);
	assert(merged.equipment(0).equipped_slot() == "PrimaryMainHand");
	assert(merged.equipment(1).equipped_slot() == "body_cloth_padded");
	assert(merged.weapon_drawn());

	auto reordered = merged;
	reordered.clear_equipment();
	*reordered.add_equipment() = merged.equipment(1);
	*reordered.add_equipment() = merged.equipment(0);
	canonicalize_avatar_visual(reordered);
	assert(same_avatar_visual(merged, reordered));

	reordered.set_weapon_drawn(false);
	assert(!same_avatar_visual(merged, reordered));

	protocol::AvatarDescriptor naked;
	const auto unequipped =
	    merge_avatar_visual(merged, naked, std::nullopt);
	assert(unequipped.equipment().empty());
	return 0;
}
