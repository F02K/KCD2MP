#pragma once

#include "kcd2mp.pb.h"

#include <Offsets/vtables/IEntitySystem.h>

#include <cstdint>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>

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
		    bool disabled,
		    std::string &error);
		void register_player_entity(std::uint32_t entity_id);
		void unregister_player_entity(std::uint32_t entity_id);
		void begin_player_spawn();
		void end_player_spawn();
		void restore_world();

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

		struct entity_state
		{
			std::uint32_t flags{};
			std::uint32_t ai_object_id{};
			bool active{};
			bool hidden{};
		};
		void ensure_sink_registered(Offsets::IEntitySystem &system);
		void isolate_entity(Offsets::IEntity *entity);
		void entity_removed(Offsets::IEntity *entity);

		isolation_sink m_sink;
		Offsets::IEntitySystem *m_sink_system{};
		std::unordered_map<std::uint32_t, entity_state> m_isolated;
		std::unordered_set<std::uint32_t> m_player_entities;
		std::uint32_t m_player_spawn_depth{};
		bool m_isolation_active{};
	};
}
