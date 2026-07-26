#include "engine_path.hpp"

#include <iostream>
#include <string>

namespace
{
	bool expect_filename(const std::string &path, const std::string &expected)
	{
		const auto actual = big::engine_path::filename_bytes(path);
		if (actual == expected)
		{
			return true;
		}

		std::cerr << "Expected byte filename of size " << expected.size() << ", got " << actual.size() << '\n';
		return false;
	}
}

int main()
{
	if (!expect_filename("levels/trosecko/objects.xml", "objects.xml")
	    || !expect_filename(R"(levels\trosecko\objects.xml)", "objects.xml")
	    || !expect_filename("objects.xml", "objects.xml")
	    || !expect_filename("levels/trosecko/", ""))
	{
		return 1;
	}

	const std::string invalid_code_page_path = std::string("levels/trosecko/") + '\xFF' + "objects.xml";
	const std::string invalid_code_page_name = std::string(1, '\xFF') + "objects.xml";
	return expect_filename(invalid_code_page_path, invalid_code_page_name) ? 0 : 1;
}
