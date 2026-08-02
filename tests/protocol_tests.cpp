#include "multiplayer/protocol.hpp"

#include <cassert>
#include <cmath>
#include <cstddef>
#include <format>
#include <limits>
#include <string>
#include <vector>

namespace
{
	kcd2mp::protocol::TransformState transform(
	    float x,
	    float speed,
	    std::uint64_t sequence)
	{
		kcd2mp::protocol::TransformState value;
		value.mutable_position()->set_x(x);
		value.mutable_position()->set_y(2.0F);
		value.mutable_position()->set_z(3.0F);
		value.mutable_rotation()->set_w(2.0F);
		value.mutable_velocity()->set_x(speed);
		value.set_sequence(sequence);
		value.set_client_time_ms(sequence * 10);
		return value;
	}
}

int main()
{
	using namespace kcd2mp;

	assert(is_valid_display_name("Henry"));
	assert(is_valid_display_name("Jindřich"));
	assert(!is_valid_display_name("ab"));
	assert(!is_valid_display_name(" leading"));
	assert(!valid_utf8_with_codepoint_count("\xC0\xAF", 1, 10));
	assert(is_valid_chat("Hello, Kuttenberg!"));
	assert(!is_valid_chat(""));

	protocol::Envelope envelope;
	auto *hello = envelope.mutable_client_hello();
	hello->set_protocol_version(protocol_version);
	hello->set_client_version(version_string);
	hello->set_whgame_timestamp(supported_whgame_timestamp);
	hello->set_whgame_image_size(supported_whgame_image_size);
	hello->set_display_name("Henry");
	hello->set_level_id("sandbox");
	auto *runtime = hello->mutable_runtime();
	runtime->set_features(required_client_runtime_capabilities);
	runtime->set_kcse_version(1);
	runtime->set_game_version(0x01050600);
	runtime->set_release_index(1);
	runtime->set_runtime_epoch(1);
	const auto &address_library = supported_address_libraries.back();
	runtime->set_address_library(address_library.build_key);
	runtime->set_address_library_distribution(address_library.distribution);
	runtime->set_address_library_format(address_library.format_version);
	runtime->set_address_library_entries(address_library.entry_count);
	runtime->set_address_library_sha256(address_library.sha256);
	std::string error;
	const auto encoded = encode(envelope, reliability::reliable, &error);
	assert(encoded);
	assert(encoded->delivery == reliability::reliable);
	const auto decoded = decode(encoded->bytes, &error);
	assert(decoded);
	assert(decoded->client_hello().display_name() == "Henry");
	auto truncated = encoded->bytes;
	truncated.pop_back();
	assert(!decode(truncated, &error));

	std::vector<std::byte> empty;
	assert(!decode(empty, &error));
	std::vector<std::byte> oversized(max_application_message_size + 1);
	assert(!decode(oversized, &error));

	auto value = transform(1.0F, 0.0F, 1);
	assert(is_finite_transform(value));
	assert(normalize_rotation(value.mutable_rotation()));
	assert(std::abs(value.rotation().w() - 1.0F) < 0.0001F);
	assert(movement_mode_for(value) == protocol::MOVEMENT_MODE_IDLE);
	value.mutable_velocity()->set_x(2.0F);
	assert(movement_mode_for(value) == protocol::MOVEMENT_MODE_WALK);
	value.mutable_velocity()->set_x(5.0F);
	assert(movement_mode_for(value) == protocol::MOVEMENT_MODE_RUN);
	value.mutable_position()->set_x(std::numeric_limits<float>::infinity());
	assert(!is_finite_transform(value));

	protocol::Envelope no_payload;
	assert(!encode(no_payload, reliability::reliable, &error));

	protocol::Envelope invalid_enum;
	auto *snapshot = invalid_enum.mutable_player_joined()->mutable_player();
	snapshot->set_player_id(1);
	snapshot->set_display_name("Henry");
	snapshot->set_movement_mode(
	    static_cast<protocol::MovementMode>(999));
	assert(!encode(invalid_enum, reliability::reliable, &error));

	protocol::Envelope too_many_players;
	auto *world = too_many_players.mutable_world_snapshot();
	for (std::size_t index = 0; index < max_players + 1; ++index)
	{
		auto *player = world->add_players();
		player->set_player_id(index + 1);
		player->set_display_name("Player" + std::to_string(index));
		player->set_movement_mode(protocol::MOVEMENT_MODE_IDLE);
	}
	assert(!encode(too_many_players, reliability::unreliable, &error));

	protocol::Envelope authentication;
	auto *credentials = authentication.mutable_client_authenticate();
	credentials->set_identity_token("token");
	credentials->set_enroll(true);
	assert(!encode(authentication, reliability::reliable, &error));

	protocol::Envelope profile_envelope;
	auto *profile_update =
	    profile_envelope.mutable_client_profile_update();
	profile_update->set_base_revision(1);
	auto *profile = profile_update->mutable_profile();
	profile->set_player_id(42);
	profile->set_revision(1);
	profile->set_display_name("Henry");
	profile->set_level_id("sandbox");
	auto *avatar = profile->mutable_avatar();
	avatar->set_archetype_id(
	    "763db0bb-4469-497d-bdc9-712b3df91b5a");
	avatar->set_revision(1);
	auto *visible_item = avatar->add_equipment();
	visible_item->set_definition_id(
	    "aaaaaaaa-aaaa-4aaa-8aaa-aaaaaaaaaaaa");
	visible_item->set_equipped_slot("PrimaryMainHand");
	profile->set_money(100);
	profile->set_money_subunits(7);
	for (const auto id : canonical_stat_ids)
	{
		auto *value = profile->add_stats();
		value->set_id(id);
		value->set_level(1);
		value->set_progress(0.0F);
	}
	for (const auto id : canonical_skill_ids)
	{
		auto *value = profile->add_skills();
		value->set_id(id);
		value->set_level(1);
		value->set_progress(0.0F);
	}
	auto *inventory_item = profile->add_inventory();
	inventory_item->set_instance_id(
	    "11111111-1111-4111-8111-111111111111");
	inventory_item->set_definition_id(visible_item->definition_id());
	inventory_item->set_count(1);
	inventory_item->set_quality(100.0F);
	inventory_item->set_condition(1.0F);
	inventory_item->set_equipped_slot(visible_item->equipped_slot());
	auto *quick_slot = profile->add_quick_access_slots();
	quick_slot->set_outfit(0);
	quick_slot->set_type(protocol::QUICK_ACCESS_SLOT_TYPE_WEAPON);
	quick_slot->set_slot(0);
	quick_slot->set_instance_id(inventory_item->instance_id());
	assert(is_valid_profile(*profile));
	auto invalid_qam_slot = *profile;
	invalid_qam_slot.mutable_quick_access_slots(0)->set_slot(8);
	assert(!is_valid_profile(invalid_qam_slot));
	auto missing_qam_item = *profile;
	missing_qam_item.mutable_quick_access_slots(0)->set_instance_id(
	    "22222222-2222-4222-8222-222222222222");
	assert(!is_valid_profile(missing_qam_item));
	auto invalid_money_subunits = *profile;
	invalid_money_subunits.set_money_subunits(
	    money_subunits_per_groschen);
	assert(!is_valid_profile(invalid_money_subunits));
	auto incomplete_profile = *profile;
	incomplete_profile.mutable_skills()->RemoveLast();
	assert(!is_valid_profile(incomplete_profile));
	auto duplicate_instance = *profile;
	*duplicate_instance.add_inventory() = duplicate_instance.inventory(0);
	assert(!is_valid_profile(duplicate_instance));
	auto invalid_condition = *profile;
	invalid_condition.mutable_inventory(0)->set_condition(1.01F);
	assert(!is_valid_profile(invalid_condition));
	const auto encoded_profile =
	    encode(profile_envelope, reliability::reliable, &error);
	assert(encoded_profile);
	const auto decoded_profile = decode(encoded_profile->bytes, &error);
	assert(decoded_profile);
	assert(decoded_profile->client_profile_update()
	    .profile()
	    .quick_access_slots_size() == 1);
	assert(
	    decoded_profile->client_profile_update()
	        .profile()
	        .avatar()
	        .archetype_id()
	    == "763db0bb-4469-497d-bdc9-712b3df91b5a");
	assert(
	    decoded_profile->client_profile_update()
	        .profile()
	        .money_subunits()
	    == 7);

	protocol::Envelope avatar_update_envelope;
	auto *avatar_update =
	    avatar_update_envelope.mutable_client_avatar_update();
	avatar_update->set_base_revision(1);
	*avatar_update->mutable_avatar() = *avatar;
	assert(encode(
	    avatar_update_envelope,
	    reliability::reliable,
	    &error));

	auto duplicate_slot = *avatar;
	*duplicate_slot.add_equipment() = duplicate_slot.equipment(0);
	duplicate_slot.mutable_equipment(1)->set_definition_id(
	    "bbbbbbbb-bbbb-4bbb-8bbb-bbbbbbbbbbbb");
	assert(!is_valid_avatar_descriptor(duplicate_slot));
	auto invalid_uuid = *avatar;
	invalid_uuid.mutable_equipment(0)->set_definition_id("runtime-item-42");
	assert(!is_valid_avatar_descriptor(invalid_uuid));
	auto invalid_slot = *avatar;
	invalid_slot.mutable_equipment(0)->set_equipped_slot("horse_body");
	assert(!is_valid_avatar_descriptor(invalid_slot));
	assert(is_valid_avatar_equipment_slot("SecondaryOffHand"));
	assert(is_valid_avatar_equipment_slot("OversizedOff"));
	assert(is_valid_avatar_equipment_slot("dlc_mantle_outer"));
	assert(!is_valid_avatar_equipment_slot("dlc mantle outer"));
	auto invalid_weapon_state = *avatar;
	invalid_weapon_state.set_weapon_class(
	    protocol::AVATAR_WEAPON_CLASS_NONE);
	invalid_weapon_state.set_weapon_drawn(true);
	assert(!is_valid_avatar_descriptor(invalid_weapon_state));
	auto oversized_avatar = *avatar;
	for (std::size_t index = oversized_avatar.equipment_size();
	     index <= max_avatar_equipment_items;
	     ++index)
	{
		auto *extra = oversized_avatar.add_equipment();
		extra->set_definition_id(std::format(
		    "aaaaaaaa-aaaa-4aaa-8aaa-{:012x}",
		    index));
		extra->set_equipped_slot("body_plate");
	}
	assert(!is_valid_avatar_descriptor(oversized_avatar));

	protocol::AvatarPolicy policy;
	policy.set_default_archetype_id(
	    "763db0bb-4469-497d-bdc9-712b3df91b5a");
	policy.add_allowed_archetype_ids(
	    "763db0bb-4469-497d-bdc9-712b3df91b5a");
	assert(is_valid_avatar_policy(policy));
	policy.add_allowed_archetype_ids(
	    "763db0bb-4469-497d-bdc9-712b3df91b5a");
	assert(!is_valid_avatar_policy(policy));

	protocol::Envelope static_avatar_in_snapshot;
	auto *static_player =
	    static_avatar_in_snapshot.mutable_world_snapshot()->add_players();
	static_player->set_player_id(1);
	static_player->set_display_name("Henry");
	*static_player->mutable_avatar() = *avatar;
	assert(!encode(
	    static_avatar_in_snapshot,
	    reliability::unreliable,
	    &error));

	protocol::Envelope entity_control;
	auto *entity_control_message =
	    entity_control.mutable_server_entity_control();
	entity_control_message->set_non_player_entities_disabled(false);
	entity_control_message->set_human_npcs_disabled(true);
	entity_control_message->set_animal_npcs_disabled(false);
	const auto encoded_control =
	    encode(entity_control, reliability::reliable, &error);
	assert(encoded_control);
	const auto decoded_control = decode(encoded_control->bytes, &error);
	assert(decoded_control);
	const auto &decoded_entity_control =
	    decoded_control->server_entity_control();
	assert(!decoded_entity_control.non_player_entities_disabled());
	assert(decoded_entity_control.has_human_npcs_disabled());
	assert(decoded_entity_control.human_npcs_disabled());
	assert(decoded_entity_control.has_animal_npcs_disabled());
	assert(!decoded_entity_control.animal_npcs_disabled());
	assert(version_string == "0.5.0");
	auto unknown_address_library = *runtime;
	unknown_address_library.set_address_library_sha256(std::string(64, '0'));
	assert(is_valid_address_library_identity(unknown_address_library));
	assert(!is_supported_address_library_identity(unknown_address_library));

	return 0;
}
