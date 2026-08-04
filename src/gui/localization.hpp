#pragma once

#include <filesystem>
#include <initializer_list>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

namespace big::ingame_ui
{
	using format_argument = std::pair<std::string_view, std::string_view>;

	[[nodiscard]] std::string normalize_language(std::string_view language);

	class localization_catalog
	{
	public:
		[[nodiscard]] bool load(
		    const std::filesystem::path &directory,
		    std::string_view language,
		    std::string &error);
		[[nodiscard]] std::string text(std::string_view key) const;
		[[nodiscard]] std::string format(
		    std::string_view key,
		    std::initializer_list<format_argument> arguments) const;
		[[nodiscard]] const std::string &language() const noexcept;

	private:
		std::string m_language{"en"};
		std::unordered_map<std::string, std::string> m_text;
	};
}
