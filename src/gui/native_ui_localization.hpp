#pragma once

#include "gui/localization.hpp"

#include <initializer_list>
#include <string>
#include <string_view>

namespace big::ingame_ui
{
	[[nodiscard]] std::string localized(std::string_view key);
	[[nodiscard]] std::string localized(
	    std::string_view key,
	    std::initializer_list<format_argument> arguments);
	[[nodiscard]] std::string active_game_language();
}
