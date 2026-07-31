#include "multiplayer/protocol.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <limits>
#include <unordered_set>

namespace kcd2mp
{
	namespace
	{
		void set_error(std::string *error, std::string message)
		{
			if (error)
			{
				*error = std::move(message);
			}
		}

		bool finite(float value)
		{
			return std::isfinite(value);
		}

		bool valid_player_snapshot(
		    const protocol::PlayerSnapshot &player,
		    bool require_avatar)
		{
			return is_valid_display_name(player.display_name())
			    && player.player_id() != 0
			    && protocol::MovementMode_IsValid(
			        static_cast<int>(player.movement_mode()))
			    && (!player.transform_valid()
			        || is_finite_transform(player.transform()))
			    && (!require_avatar || player.has_avatar())
			    && (!player.has_avatar()
			        || is_valid_avatar_descriptor(player.avatar()));
		}

		bool valid_identifier(std::string_view value, std::size_t maximum = 128)
		{
			return valid_utf8_with_codepoint_count(value, 1, maximum);
		}

		bool valid_envelope(const protocol::Envelope &envelope)
		{
			if (envelope.has_client_hello())
			{
				const auto &message = envelope.client_hello();
				return message.protocol_version() == protocol_version
				    && message.client_version() == version_string
				    && message.whgame_timestamp()
				        == supported_whgame_timestamp
				    && message.whgame_image_size()
				        == supported_whgame_image_size
				    && is_valid_display_name(message.display_name())
				    && message.password().size() <= 256
				    && message.content_hash().size() <= 64
				    && message.resume_token().size() <= 128
				    && message.has_runtime()
				    && (message.runtime().features()
				            & ~known_client_runtime_capabilities)
				        == 0
				    && message.runtime().runtime_epoch() != 0
				    && message.runtime().kcse_version() != 0
				    && message.runtime().game_version()
				        == supported_kcse_game_version
				    && message.runtime().release_index()
				        == supported_kcse_release_index
				    && valid_identifier(
				        message.runtime().address_library(),
				        64);
			}
			if (envelope.has_server_challenge())
			{
				return valid_identifier(envelope.server_challenge().server_id());
			}
			if (envelope.has_client_authenticate())
			{
				const auto &message = envelope.client_authenticate();
				const auto credential_count =
				    static_cast<int>(!message.identity_token().empty())
				    + static_cast<int>(!message.claim_code().empty())
				    + static_cast<int>(message.enroll());
				return credential_count == 1
				    && message.identity_token().size() <= 128
				    && message.claim_code().size() <= 64
				    && message.resume_token().size() <= 128;
			}
			if (envelope.has_server_bootstrap())
			{
				const auto &message = envelope.server_bootstrap();
				return valid_identifier(message.server_id())
				    && valid_identifier(message.session_id())
				    && valid_identifier(message.level_id())
				    && message.manifest_revision() > 0
				    && message.timeout_seconds() >= 30
				    && message.timeout_seconds() <= 600
				    && protocol::BootstrapMode_IsValid(
				        static_cast<int>(message.mode()))
				    && (!message.spawn_valid()
				        || (message.has_spawn()
				            && is_finite_transform(message.spawn())))
				    && (!message.has_profile()
				        || is_valid_profile(message.profile()))
				    && message.issued_identity_token().size() <= 128;
			}
			if (envelope.has_client_world_ready())
			{
				const auto &message = envelope.client_world_ready();
				return valid_identifier(message.session_id())
				    && valid_identifier(message.level_id())
				    && message.manifest_revision() > 0
				    && message.has_avatar()
				    && is_valid_avatar_descriptor(message.avatar())
				    && (!message.initialized_session()
				        || (message.has_initial_spawn()
				            && is_finite_transform(message.initial_spawn())));
			}
			if (envelope.has_client_world_failed())
			{
				const auto &message = envelope.client_world_failed();
				return valid_identifier(message.session_id())
				    && valid_utf8_with_codepoint_count(
				        message.reason(), 1, 512);
			}
			if (envelope.has_client_profile_update())
			{
				return envelope.client_profile_update().has_profile()
				    && envelope.client_profile_update().base_revision() > 0
				    && is_valid_profile(
				        envelope.client_profile_update().profile());
			}
			if (envelope.has_profile_accepted())
			{
				return envelope.profile_accepted().revision() > 0;
			}
			if (envelope.has_profile_rejected())
			{
				return envelope.profile_rejected().authoritative_revision() > 0
				    && valid_utf8_with_codepoint_count(
				        envelope.profile_rejected().reason(), 1, 512);
			}
			if (envelope.has_client_avatar_update())
			{
				const auto &message = envelope.client_avatar_update();
				return message.base_revision() > 0 && message.has_avatar()
				    && message.avatar().revision() == message.base_revision();
			}
			if (envelope.has_avatar_accepted())
			{
				return envelope.avatar_accepted().revision() > 0;
			}
			if (envelope.has_avatar_rejected())
			{
				const auto &message = envelope.avatar_rejected();
				return message.has_authoritative_avatar()
				    && is_valid_avatar_descriptor(
				        message.authoritative_avatar())
				    && valid_utf8_with_codepoint_count(
				        message.reason(), 1, 512);
			}
			if (envelope.has_player_avatar_updated())
			{
				const auto &message = envelope.player_avatar_updated();
				return message.player_id() != 0 && message.has_avatar()
				    && is_valid_avatar_descriptor(message.avatar());
			}
			if (envelope.has_server_accepted())
			{
				const auto &message = envelope.server_accepted();
				if (message.players_size() > static_cast<int>(max_players)
				    || message.profile_snapshot_interval_seconds() < 5
				    || message.profile_snapshot_interval_seconds() > 60
				    || !message.has_avatar_policy()
				    || !is_valid_avatar_policy(message.avatar_policy()))
				{
					return false;
				}
				for (const auto &player : message.players())
				{
					if (!valid_player_snapshot(player, true))
					{
						return false;
					}
				}
			}
			else if (envelope.has_server_rejected())
			{
				return protocol::RejectReason_IsValid(
				    static_cast<int>(envelope.server_rejected().reason()));
			}
			else if (envelope.has_player_joined())
			{
				return valid_player_snapshot(
				    envelope.player_joined().player(),
				    true);
			}
			else if (envelope.has_client_transform())
			{
				return envelope.client_transform().has_transform()
				    && is_finite_transform(
				        envelope.client_transform().transform());
			}
			else if (envelope.has_world_snapshot())
			{
				const auto &message = envelope.world_snapshot();
				if (message.players_size() > static_cast<int>(max_players))
				{
					return false;
				}
				for (const auto &player : message.players())
				{
					if (!valid_player_snapshot(player, false)
					    || player.has_avatar())
					{
						return false;
					}
				}
			}
			else if (envelope.has_state_correction())
			{
				return envelope.state_correction().has_accepted_transform()
				    && is_finite_transform(
				        envelope.state_correction().accepted_transform());
			}
			else if (envelope.has_chat_send())
			{
				return is_valid_chat(envelope.chat_send().text());
			}
			else if (envelope.has_chat_broadcast())
			{
				return is_valid_display_name(
				           envelope.chat_broadcast().display_name())
				    && is_valid_chat(envelope.chat_broadcast().text());
			}
			return true;
		}
	}

	std::optional<encoded_message> encode(
	    const protocol::Envelope &envelope,
	    reliability delivery,
	    std::string *error)
	{
		if (envelope.payload_case() == protocol::Envelope::PAYLOAD_NOT_SET)
		{
			set_error(error, "envelope has no payload");
			return std::nullopt;
		}
		if (!valid_envelope(envelope))
		{
			set_error(error, "envelope violates protocol limits");
			return std::nullopt;
		}

		const auto size = envelope.ByteSizeLong();
		if (size == 0 || size > max_application_message_size
		    || size > static_cast<std::size_t>(std::numeric_limits<int>::max()))
		{
			set_error(error, "encoded message exceeds the application limit");
			return std::nullopt;
		}

		encoded_message result;
		result.delivery = delivery;
		result.bytes.resize(size);
		if (!envelope.SerializeToArray(result.bytes.data(), static_cast<int>(result.bytes.size())))
		{
			set_error(error, "protobuf serialization failed");
			return std::nullopt;
		}
		return result;
	}

	std::optional<protocol::Envelope> decode(
	    std::span<const std::byte> bytes,
	    std::string *error)
	{
		if (bytes.empty() || bytes.size() > max_application_message_size
		    || bytes.size() > static_cast<std::size_t>(std::numeric_limits<int>::max()))
		{
			set_error(error, "message size is invalid");
			return std::nullopt;
		}

		protocol::Envelope envelope;
		if (!envelope.ParseFromArray(bytes.data(), static_cast<int>(bytes.size()))
		    || envelope.payload_case() == protocol::Envelope::PAYLOAD_NOT_SET)
		{
			set_error(error, "protobuf payload is malformed or empty");
			return std::nullopt;
		}
		if (!valid_envelope(envelope))
		{
			set_error(error, "envelope violates protocol limits");
			return std::nullopt;
		}
		return envelope;
	}

	bool valid_utf8_with_codepoint_count(
	    std::string_view value,
	    std::size_t minimum,
	    std::size_t maximum)
	{
		std::size_t codepoints = 0;
		for (std::size_t index = 0; index < value.size();)
		{
			const auto lead = static_cast<unsigned char>(value[index]);
			std::size_t length = 0;
			std::uint32_t codepoint = 0;
			if (lead <= 0x7F)
			{
				length = 1;
				codepoint = lead;
			}
			else if ((lead & 0xE0) == 0xC0)
			{
				length = 2;
				codepoint = lead & 0x1F;
			}
			else if ((lead & 0xF0) == 0xE0)
			{
				length = 3;
				codepoint = lead & 0x0F;
			}
			else if ((lead & 0xF8) == 0xF0)
			{
				length = 4;
				codepoint = lead & 0x07;
			}
			else
			{
				return false;
			}

			if (index + length > value.size())
			{
				return false;
			}
			for (std::size_t continuation = 1; continuation < length; ++continuation)
			{
				const auto byte = static_cast<unsigned char>(value[index + continuation]);
				if ((byte & 0xC0) != 0x80)
				{
					return false;
				}
				codepoint = (codepoint << 6) | (byte & 0x3F);
			}

			const bool overlong = (length == 2 && codepoint < 0x80)
			    || (length == 3 && codepoint < 0x800)
			    || (length == 4 && codepoint < 0x10000);
			if (overlong || codepoint > 0x10FFFF
			    || (codepoint >= 0xD800 && codepoint <= 0xDFFF)
			    || codepoint == 0)
			{
				return false;
			}
			++codepoints;
			if (codepoints > maximum)
			{
				return false;
			}
			index += length;
		}
		return codepoints >= minimum && codepoints <= maximum;
	}

	bool is_valid_display_name(std::string_view value)
	{
		if (!valid_utf8_with_codepoint_count(
		        value,
		        min_display_name_codepoints,
		        max_display_name_codepoints))
		{
			return false;
		}
		return value.front() != ' ' && value.back() != ' ';
	}

	bool is_valid_chat(std::string_view value)
	{
		return valid_utf8_with_codepoint_count(value, 1, max_chat_codepoints);
	}

	bool is_uuid(std::string_view value)
	{
		if (value.size() != 36)
			return false;
		for (std::size_t index = 0; index < value.size(); ++index)
		{
			if (index == 8 || index == 13 || index == 18 || index == 23)
			{
				if (value[index] != '-')
					return false;
				continue;
			}
			const auto character =
			    static_cast<unsigned char>(value[index]);
			if (!std::isxdigit(character))
				return false;
		}
		return true;
	}

	bool is_valid_avatar_equipment_slot(std::string_view value)
	{
		static constexpr std::array slots{
		    std::string_view{"body_coat"},
		    std::string_view{"gloves"},
		    std::string_view{"ring"},
		    std::string_view{"necklace"},
		    std::string_view{"collar"},
		    std::string_view{"head_hood"},
		    std::string_view{"boot"},
		    std::string_view{"head_coif"},
		    std::string_view{"head_coif_padded"},
		    std::string_view{"head_cap"},
		    std::string_view{"head_helmet"},
		    std::string_view{"body_cloth"},
		    std::string_view{"body_cloth_padded"},
		    std::string_view{"body_chainmail"},
		    std::string_view{"body_plate"},
		    std::string_view{"sleeves"},
		    std::string_view{"leg_trousers"},
		    std::string_view{"leg_trousers_padded"},
		    std::string_view{"leg_armor"},
		    std::string_view{"spur"},
		    std::string_view{"belt"},
		    std::string_view{"pouch"},
		    std::string_view{"PrimaryMainHand"},
		    std::string_view{"PrimaryOffHand"},
		    std::string_view{"SecondaryMainHand"},
		    std::string_view{"Dagger"},
		    std::string_view{"Torch"},
		    std::string_view{"Oversized"}};
		return std::ranges::find(slots, value) != slots.end();
	}

	bool is_valid_avatar_descriptor(
	    const protocol::AvatarDescriptor &avatar)
	{
		if (!is_uuid(avatar.archetype_id())
		    || avatar.revision() == 0
		    || avatar.equipment_size()
		        > static_cast<int>(max_avatar_equipment_items)
		    || !protocol::AvatarStance_IsValid(
		        static_cast<int>(avatar.stance()))
		    || !protocol::AvatarWeaponClass_IsValid(
		        static_cast<int>(avatar.weapon_class()))
		    || (avatar.weapon_drawn()
		        && avatar.weapon_class()
		            == protocol::AVATAR_WEAPON_CLASS_NONE))
		{
			return false;
		}
		std::unordered_set<std::string> slots;
		return std::ranges::all_of(
		    avatar.equipment(),
		    [&](const protocol::AvatarEquipment &item)
		    {
			    return is_uuid(item.definition_id())
			        && is_valid_avatar_equipment_slot(
			            item.equipped_slot())
			        && slots.insert(item.equipped_slot()).second;
		    });
	}

	bool is_valid_avatar_policy(const protocol::AvatarPolicy &policy)
	{
		if (!is_uuid(policy.default_archetype_id())
		    || policy.allowed_archetype_ids_size() == 0
		    || policy.allowed_archetype_ids_size()
		        > static_cast<int>(max_avatar_archetypes))
		{
			return false;
		}
		bool contains_default = false;
		std::unordered_set<std::string> archetypes;
		for (const auto &archetype : policy.allowed_archetype_ids())
		{
			if (!is_uuid(archetype)
			    || !archetypes.insert(archetype).second)
			{
				return false;
			}
			contains_default =
			    contains_default || archetype == policy.default_archetype_id();
		}
		return contains_default;
	}

	bool is_valid_profile(const protocol::PlayerProfile &profile)
	{
		if (profile.player_id() == 0 || profile.revision() == 0
		    || !is_valid_display_name(profile.display_name())
		    || !valid_identifier(profile.level_id())
		    || profile.money() < 0 || profile.money() > max_profile_money
		    || profile.money_subunits() >= money_subunits_per_groschen
		    || profile.stats_size() != static_cast<int>(profile_stat_count)
		    || profile.skills_size() != static_cast<int>(profile_skill_count)
		    || profile.inventory_size()
		        > static_cast<int>(max_profile_inventory_items)
		    || (profile.transform_valid()
		        && (!profile.has_last_transform()
		            || !is_finite_transform(profile.last_transform())))
		    || !profile.has_avatar()
		    || !is_valid_avatar_descriptor(profile.avatar()))
		{
			return false;
		}
		const auto valid_rpg = [](const protocol::RpgValue &value)
		{
			return value.level() >= 0 && value.level() <= 100
			    && std::isfinite(value.progress())
			    && value.progress() >= 0.0F && value.progress() <= 1.0F;
		};
		const auto exact_rpg_set = [&](const auto &values, const auto &ids)
		{
			std::unordered_set<std::string_view> found;
			for (const auto &value : values)
			{
				if (!valid_rpg(value)
				    || std::ranges::find(ids, value.id()) == ids.end()
				    || !found.insert(value.id()).second)
				{
					return false;
				}
			}
			return found.size() == ids.size();
		};
		if (!exact_rpg_set(profile.stats(), canonical_stat_ids)
		    || !exact_rpg_set(profile.skills(), canonical_skill_ids))
		{
			return false;
		}

		std::unordered_set<std::string> instance_ids;
		std::unordered_set<std::string> equipped_slots;
		for (const auto &item : profile.inventory())
		{
			if (!is_uuid(item.instance_id())
			    || !instance_ids.insert(item.instance_id()).second
			    || !is_uuid(item.definition_id())
			    || item.count() == 0 || item.count() > max_profile_item_count
			    || !std::isfinite(item.quality())
			    || item.quality() < 0.0F || item.quality() > 100.0F
			    || !std::isfinite(item.condition())
			    || item.condition() < 0.0F || item.condition() > 1.0F)
			{
				return false;
			}
			if (item.has_equipped_slot()
			    && (!is_valid_avatar_equipment_slot(item.equipped_slot())
			        || !equipped_slots.insert(item.equipped_slot()).second))
			{
				return false;
			}
		}
		for (const auto &visible : profile.avatar().equipment())
		{
			const auto match = std::ranges::find_if(
			    profile.inventory(),
			    [&](const protocol::InventoryItem &item)
			    {
				    return item.definition_id() == visible.definition_id()
				        && item.has_equipped_slot()
				        && item.equipped_slot() == visible.equipped_slot();
			    });
			if (match == profile.inventory().end())
				return false;
		}
		return true;
	}

	bool is_finite_transform(const protocol::TransformState &transform)
	{
		if (!transform.has_position() || !transform.has_rotation() || !transform.has_velocity())
		{
			return false;
		}
		const auto &position = transform.position();
		const auto &rotation = transform.rotation();
		const auto &velocity = transform.velocity();
		return finite(position.x()) && finite(position.y()) && finite(position.z())
		    && finite(rotation.x()) && finite(rotation.y()) && finite(rotation.z())
		    && finite(rotation.w()) && finite(velocity.x()) && finite(velocity.y())
		    && finite(velocity.z());
	}

	bool normalize_rotation(protocol::Quaternion *rotation)
	{
		if (!rotation)
		{
			return false;
		}
		const auto length_squared = rotation->x() * rotation->x()
		    + rotation->y() * rotation->y()
		    + rotation->z() * rotation->z()
		    + rotation->w() * rotation->w();
		if (!std::isfinite(length_squared) || length_squared < 0.000001F)
		{
			return false;
		}
		const auto inverse_length = 1.0F / std::sqrt(length_squared);
		rotation->set_x(rotation->x() * inverse_length);
		rotation->set_y(rotation->y() * inverse_length);
		rotation->set_z(rotation->z() * inverse_length);
		rotation->set_w(rotation->w() * inverse_length);
		return true;
	}

	protocol::MovementMode movement_mode_for(
	    const protocol::TransformState &transform)
	{
		if (!transform.has_velocity())
		{
			return protocol::MOVEMENT_MODE_IDLE;
		}
		const auto horizontal_speed = std::hypot(
		    transform.velocity().x(),
		    transform.velocity().y());
		if (horizontal_speed < 0.15F)
		{
			return protocol::MOVEMENT_MODE_IDLE;
		}
		if (horizontal_speed < 3.2F)
		{
			return protocol::MOVEMENT_MODE_WALK;
		}
		return protocol::MOVEMENT_MODE_RUN;
	}
}
