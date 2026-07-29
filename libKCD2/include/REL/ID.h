#pragma once

#include "REL/Module.h"

#include <cstdint>
#include <string>
#include <vector>

namespace REL
{
	class IDDatabase
	{
	public:
		[[nodiscard]] static IDDatabase& get();

		// abstract id -> this binary's offset. Fatal if the id is absent (the plugin
		// references an address the current build's table doesn't cover).
		[[nodiscard]] std::size_t id2offset(std::uint64_t a_id) const;

		// Must be called before the first get() to override the default
		// (<game_root>/KCSE/addresslib/). No-op afterwards.
		static void SetDatabaseDirectory(std::wstring a_dir);

	private:
		struct mapping_t
		{
			std::uint32_t id;
			std::uint32_t offset;
		};

		IDDatabase();
		IDDatabase(const IDDatabase&) = delete;
		IDDatabase(IDDatabase&&) = delete;
		~IDDatabase() = default;
		IDDatabase& operator=(const IDDatabase&) = delete;
		IDDatabase& operator=(IDDatabase&&) = delete;

		void load();

		std::vector<mapping_t> _id2offset;  // sorted by id
	};

	class ID
	{
	public:
		constexpr ID() noexcept = default;

		explicit constexpr ID(std::uint64_t a_id) noexcept :
			_id(a_id) {}

		constexpr ID& operator=(std::uint64_t a_id) noexcept
		{
			_id = a_id;
			return *this;
		}

		[[nodiscard]] std::uintptr_t          address() const { return base() + offset(); }
		[[nodiscard]] constexpr std::uint64_t id() const noexcept { return _id; }
		[[nodiscard]] std::size_t             offset() const { return IDDatabase::get().id2offset(_id); }

	private:
		[[nodiscard]] static std::uintptr_t base() { return Module::get().base(); }

		std::uint64_t _id{ 0 };
	};
}
