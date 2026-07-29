#pragma once

#include "kcd2mp.pb.h"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "npc/catalog.hpp"

namespace kcd2mp
{
	constexpr std::uint32_t protocol_version = 3;
	constexpr std::string_view version_string = "0.3.0";
	constexpr std::uint32_t supported_whgame_timestamp = 0x6A350E20;
	constexpr std::uint64_t supported_whgame_image_size = 0x5B2D000;
	constexpr std::size_t max_application_message_size = 64 * 1024;
	constexpr std::size_t max_players = 8;
	constexpr std::size_t max_display_name_codepoints = 32;
	constexpr std::size_t min_display_name_codepoints = 3;
	constexpr std::size_t max_chat_codepoints = 256;
	constexpr std::size_t max_profile_rpg_values = 128;
	constexpr std::size_t max_profile_inventory_items = 512;
	constexpr std::size_t max_avatar_equipment_items = 32;
	constexpr std::size_t max_avatar_archetypes = 32;
	constexpr std::string_view default_avatar_archetype_id =
	    npc::default_soul_id;

	using player_id = std::uint64_t;
	using connection_id = std::uint64_t;

	enum class reliability
	{
		unreliable,
		reliable
	};

	struct encoded_message
	{
		std::vector<std::byte> bytes;
		reliability delivery{reliability::reliable};
	};

	[[nodiscard]] std::optional<encoded_message> encode(
	    const protocol::Envelope &envelope,
	    reliability delivery,
	    std::string *error = nullptr);
	[[nodiscard]] std::optional<protocol::Envelope> decode(
	    std::span<const std::byte> bytes,
	    std::string *error = nullptr);
	[[nodiscard]] bool valid_utf8_with_codepoint_count(
	    std::string_view value,
	    std::size_t minimum,
	    std::size_t maximum);
	[[nodiscard]] bool is_valid_display_name(std::string_view value);
	[[nodiscard]] bool is_valid_chat(std::string_view value);
	[[nodiscard]] bool is_uuid(std::string_view value);
	[[nodiscard]] bool is_valid_avatar_equipment_slot(
	    std::string_view value);
	[[nodiscard]] bool is_valid_profile(const protocol::PlayerProfile &profile);
	[[nodiscard]] bool is_valid_avatar_descriptor(
	    const protocol::AvatarDescriptor &avatar);
	[[nodiscard]] bool is_valid_avatar_policy(
	    const protocol::AvatarPolicy &policy);
	[[nodiscard]] bool is_finite_transform(const protocol::TransformState &transform);
	[[nodiscard]] bool normalize_rotation(protocol::Quaternion *rotation);
	[[nodiscard]] protocol::MovementMode movement_mode_for(
	    const protocol::TransformState &transform);
}
