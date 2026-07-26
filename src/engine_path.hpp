#pragma once

#include <string>
#include <string_view>

namespace big::engine_path
{
	// WHGame exposes paths as opaque byte strings. Keep them byte-preserving:
	// std::filesystem::path(const char*) converts through a Windows code page.
	[[nodiscard]] inline std::string filename_bytes(const std::string_view path)
	{
		const auto separator = path.find_last_of("/\\");
		return std::string(separator == std::string_view::npos ? path : path.substr(separator + 1));
	}
}
