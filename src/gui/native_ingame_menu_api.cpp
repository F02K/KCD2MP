#include "gui/native_ingame_menu_api.hpp"

#include <Offsets/vtables/IUIElement.h>
#include <guimodule/C_UIMenu.h>
#include <guimodule/SUITypes.h>

#include <cstddef>

namespace big::ingame_ui
{
	namespace
	{
		constexpr std::ptrdiff_t element_offset = 0x48;
		constexpr std::ptrdiff_t menu_interface_offset = 0x58;
		constexpr std::ptrdiff_t menu_state_offset = 0xA0;
		constexpr std::ptrdiff_t menu_page_offset = 0xA1;

		Offsets::IUIElement *element_from(void *menu) noexcept
		{
			return menu
			    ? *reinterpret_cast<Offsets::IUIElement **>(
			          static_cast<std::byte *>(menu) + element_offset)
			    : nullptr;
		}
	}

	native_menu_api::native_menu_api(void *menu) noexcept
	    : m_menu(menu), m_element(element_from(menu))
	{
	}

	bool native_menu_api::available() const noexcept
	{
		return m_menu && m_element;
	}

	Offsets::IUIElement *native_menu_api::element() const noexcept
	{
		return m_element;
	}

	std::uint8_t native_menu_api::current_page() const noexcept
	{
		return m_menu
		    ? *reinterpret_cast<const std::uint8_t *>(
		          static_cast<const std::byte *>(m_menu) + menu_page_offset)
		    : 0;
	}

	std::uint8_t native_menu_api::mode() const noexcept
	{
		return m_menu
		    ? *reinterpret_cast<const std::uint8_t *>(
		          static_cast<const std::byte *>(m_menu) + menu_state_offset)
		    : 0;
	}

	bool native_menu_api::call(
	    const char *function,
	    const SUIArguments &arguments) const
	{
		return m_element
		    && m_element->CallFunction(function, arguments, nullptr, nullptr);
	}

	bool native_menu_api::append_button(const button &value) const
	{
		if (!available() || value.id.empty())
			return false;
		SUIArguments arguments;
		arguments.AddArgument(value.id.c_str());
		arguments.AddArgument(value.container);
		arguments.AddArgument(value.text.c_str());
		arguments.AddArgument(value.tooltip.c_str());
		arguments.AddArgument(value.disabled);
		return call("AddBasicButton", arguments);
	}

	bool native_menu_api::show(const page &value) const
	{
		if (!available() || !value.valid())
			return false;

		*reinterpret_cast<std::uint8_t *>(
		    static_cast<std::byte *>(m_menu) + menu_page_offset) = value.id;
		SUIArguments empty;
		bool rendered = call("ClearAll", empty);

		SUIArguments color;
		color.AddArgument(static_cast<int>(mode()));
		rendered = call("SetMenuColor", color) && rendered;

		SUIArguments prepare;
		prepare.AddArgument(value.width);
		prepare.AddArgument(value.height);
		prepare.AddArgument(value.visible_rows);
		prepare.AddArgument(value.title.c_str());
		prepare.AddArgument(value.style);
		rendered = call("PreparePage", prepare) && rendered;

		for (const auto &item : value.buttons)
			rendered = append_button(item) && rendered;
		if (value.finalize)
			rendered = call("ShowPage", empty) && rendered;
		if (!value.selected_button.empty())
		{
			SUIArguments select;
			select.AddArgument(value.selected_button.c_str());
			select.AddArgument(0);
			rendered = call("SelectButton", select) && rendered;
		}
		return rendered;
	}

	void native_menu_api::close() const
	{
		if (!m_menu)
			return;
		auto *interface_pointer =
		    static_cast<std::byte *>(m_menu) + menu_interface_offset;
		auto **vtable = *reinterpret_cast<void ***>(interface_pointer);
		using close_function = void(__fastcall *)(void *);
		reinterpret_cast<close_function>(vtable[2])(interface_pointer);
	}

	bool native_menu_api::open_root() const
	{
		if (!m_menu)
			return false;

		using wh::guimodule::E_ButtonId;
		E_ButtonId::Type selection{};
		switch (mode())
		{
		case 1:
		case 4:
			selection = E_ButtonId::Continue;
			break;
		case 2:
		case 3:
			selection = E_ButtonId::Resume;
			break;
		default:
			return false;
		}

		reinterpret_cast<wh::guimodule::C_UIMenu *>(m_menu)
		    ->RebuildRootPage(selection);
		return true;
	}
}
