#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

namespace REL
{
	enum class Distribution : std::uint32_t
	{
		Unknown = 0,
		Steam = 1,
		GOG = 2,
		Epic = 3,
	};

	[[nodiscard]] std::string_view to_string(Distribution a_dist) noexcept;

	// One PE section of the loaded module.
	class Segment
	{
	public:
		enum Name : std::size_t
		{
			textx,  // .text (executable)
			idata,
			rdata,
			data,
			pdata,
			total
		};

		Segment() noexcept = default;
		Segment(std::uintptr_t a_base, std::uintptr_t a_address, std::size_t a_size) noexcept :
			_base(a_base), _address(a_address), _size(a_size) {}

		[[nodiscard]] std::uintptr_t address() const noexcept { return _address; }
		[[nodiscard]] std::size_t    offset() const noexcept { return _address - _base; }
		[[nodiscard]] std::size_t    size() const noexcept { return _size; }
		[[nodiscard]] void*          pointer() const noexcept { return reinterpret_cast<void*>(_address); }

		template <class T>
		[[nodiscard]] T* pointer() const noexcept { return static_cast<T*>(pointer()); }

	private:
		std::uintptr_t _base{ 0 };
		std::uintptr_t _address{ 0 };
		std::size_t    _size{ 0 };
	};

	// Singleton describing the loaded WHGame.dll. Mirrors REL::Module::get().
	class Module
	{
	public:
		[[nodiscard]] static Module& get();

		[[nodiscard]] std::uintptr_t      base() const noexcept { return _base; }
		[[nodiscard]] Distribution        distribution() const noexcept { return _distribution; }
		[[nodiscard]] std::uint32_t       timestamp() const noexcept { return _timestamp; }  // PE TimeDateStamp
		[[nodiscard]] const std::wstring& game_root() const noexcept { return _gameRoot; }

		// Lazily parsed from game-root config files on first call (empty on failure).
		[[nodiscard]] std::string_view release();     // system.cfg     -> "1.9.8"
		[[nodiscard]] std::string_view build_code();  // whdlversions.txt -> "404-504czj4"

		[[nodiscard]] Segment segment(Segment::Name a_name) const noexcept { return _segments[a_name]; }

		[[nodiscard]] void* pointer() const noexcept { return reinterpret_cast<void*>(_base); }
		template <class T>
		[[nodiscard]] T* pointer() const noexcept { return static_cast<T*>(pointer()); }

		static constexpr std::wstring_view FILENAME = L"WHGame.dll";

	private:
		Module();
		Module(const Module&) = delete;
		Module(Module&&) = delete;
		~Module() = default;
		Module& operator=(const Module&) = delete;
		Module& operator=(Module&&) = delete;

		void resolve_game_root();
		void load_pe();           // base timestamp + segments
		void detect_distribution();

		std::array<Segment, Segment::total> _segments{};
		std::wstring                        _gameRoot{};
		std::string                         _release{};
		std::string                         _buildCode{};
		std::uintptr_t                      _base{ 0 };
		std::uint32_t                       _timestamp{ 0 };
		Distribution                        _distribution{ Distribution::Unknown };
		bool                                _releaseQueried{ false };
		bool                                _buildQueried{ false };
	};
}
