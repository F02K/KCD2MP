#pragma once

#include "gui/ingame_menu.hpp"

#include <cstdint>

namespace Offsets
{
	struct IUIElement;
}

struct SUIArguments;

namespace big::ingame_ui
{
	// Adapter from the UI-neutral page model to KCD2's native Scaleform menu.
	// It owns no multiplayer state and can therefore be reused by other menus.
	class native_menu_api
	{
	public:
		explicit native_menu_api(void *menu) noexcept;

		[[nodiscard]] bool available() const noexcept;
		[[nodiscard]] Offsets::IUIElement *element() const noexcept;
		[[nodiscard]] std::uint8_t current_page() const noexcept;
		[[nodiscard]] std::uint8_t mode() const noexcept;
		[[nodiscard]] bool append_button(const button &value) const;
		[[nodiscard]] bool show(const page &value) const;
		void close() const;
		void open_root() const;

	private:
		[[nodiscard]] bool call(
		    const char *function,
		    const SUIArguments &arguments) const;

		void *m_menu{};
		Offsets::IUIElement *m_element{};
	};
}
