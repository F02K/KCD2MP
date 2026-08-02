#pragma once

#include "kcd2mp.pb.h"

#include <Offsets/vtables/IEntitySystem.h>

#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <vector>

namespace Offsets
{
	struct IEntity;
}

namespace wh::entitymodule
{
	class C_Actor;
}

namespace kcd2mp::kcse
{
	struct native_player_view
	{
		Offsets::IEntity *entity{};
		wh::entitymodule::C_Actor *actor{};
	};

	class native_entity_backend
	{
	public:
		native_entity_backend();
		~native_entity_backend();

		[[nodiscard]] native_player_view player() const;
		[[nodiscard]] std::optional<protocol::TransformState> read_transform(
		    Offsets::IEntity *entity) const;
		[[nodiscard]] bool write_transform(
		    Offsets::IEntity *entity,
		    const protocol::TransformState &transform,
		    std::string &error) const;
		[[nodiscard]] bool set_world_isolated(
		    bool humans_disabled,
		    bool animals_disabled,
		    std::string &error);
		void register_player_entity(std::uint32_t entity_id);
		void unregister_player_entity(std::uint32_t entity_id);
		void begin_player_spawn();
		void end_player_spawn();
		void process_pending_isolation();
		[[nodiscard]] bool begin_world_sync(std::string &error);
		void restore_world();
		[[nodiscard]] std::vector<protocol::WorldObjectState>
		poll_world_object_updates();
		[[nodiscard]] bool apply_world_object_state(
		    const protocol::WorldObjectState &state,
		    std::string &error);
		void reset_world_sync();

	private:
		class isolation_sink final : public Offsets::IEntitySystemSink
		{
		public:
			void attach(native_entity_backend &owner);
			bool OnBeforeSpawn(void *params) override;
			void OnSpawn(Offsets::IEntity *entity, void *params) override;
			bool OnRemove(Offsets::IEntity *entity) override;
			void OnReused(Offsets::IEntity *entity, void *params) override;
			void _vf5(Offsets::IEntity *entity, void *event) override;
			void OnEvent(Offsets::IEntity *entity, void *event) override;
			void GetMemoryUsage(void *sizer) const override;

		private:
			native_entity_backend *m_owner{};
		};

		// CryEngine declares OnAfterInit before the virtual destructor, so this
		// order is ABI-significant for the two-slot IGameObjectSystemSink vtable.
		class game_object_init_sink final
		{
		public:
			virtual void OnAfterInit(void *game_object);
			virtual ~game_object_init_sink() = default;
			void attach(native_entity_backend &owner);

		private:
			native_entity_backend *m_owner{};
		};

		struct entity_state
		{
			bool hidden{};
		};
		struct pending_entity
		{
			std::uint16_t waited_frames{};
			bool game_object_initialized{};
		};
		void ensure_sink_registered(Offsets::IEntitySystem &system);
		void ensure_game_object_sink_registered(void *system);
		void queue_entity_for_isolation(
		    Offsets::IEntity *entity,
		    bool game_object_initialized,
		    bool actor_class_confirmed);
		void game_object_initialized(std::uint32_t entity_id);
		void refresh_local_player_exclusion(Offsets::IEntitySystem &system);
		void refresh_actor_roster(Offsets::IEntitySystem &system);
		void maintain_isolated_entities(Offsets::IEntitySystem &system);
		[[nodiscard]] bool isolate_entity(Offsets::IEntity *entity);
		[[nodiscard]] bool should_isolate_actor(
		    Offsets::IEntity *entity) const;
		void entity_removed(Offsets::IEntity *entity);
		void entity_event(Offsets::IEntity *entity, void *event);
		[[nodiscard]] std::optional<protocol::WorldObjectState>
		capture_world_object(Offsets::IEntity *entity) const;
		[[nodiscard]] bool apply_world_inventory(
		    Offsets::IEntity *entity,
		    const protocol::WorldObjectState &state,
		    std::string &error) const;

		isolation_sink m_sink;
		game_object_init_sink m_game_object_sink;
		Offsets::IEntitySystem *m_sink_system{};
		void *m_game_object_system{};
		std::unordered_map<std::uint32_t, entity_state> m_isolated;
		std::unordered_set<std::uint32_t> m_player_entities;
		std::unordered_map<std::uint32_t, pending_entity> m_pending_isolation;
		std::uint32_t m_local_player_entity_id{};
		std::uint32_t m_player_spawn_depth{};
		std::uint32_t m_isolation_maintenance_frame{};
		int m_last_actor_count{-1};
		bool m_human_npcs_disabled{};
		bool m_animal_npcs_disabled{};
		bool m_isolation_active{};
		bool m_applying_world_state{};
		std::uint32_t m_world_poll_frame{};
		std::unordered_set<std::uint64_t> m_open_world_containers;
		std::unordered_map<std::uint64_t, protocol::WorldObjectState>
		    m_last_world_observations;
		std::unordered_map<std::uint64_t, protocol::WorldObjectState>
		    m_deferred_world_states;
		std::deque<protocol::WorldObjectState> m_world_updates;
	};
}
