#include "REL/ID.h"

#include "REL/Module.h"

#include <Windows.h>

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>

namespace REL
{
	namespace
	{
		std::wstring s_dbDir;
		bool         s_dbDirSet = false;

		[[noreturn]] void fail(const std::string& a_msg)
		{
			::MessageBoxA(nullptr, a_msg.c_str(), "kcd_re REL::IDDatabase", MB_OK | MB_ICONERROR);
			::TerminateProcess(::GetCurrentProcess(), 1);
		}
	}

	void IDDatabase::SetDatabaseDirectory(std::wstring a_dir)
	{
		s_dbDir = std::move(a_dir);
		s_dbDirSet = true;
	}

	IDDatabase& IDDatabase::get()
	{
		static IDDatabase singleton;
		return singleton;
	}

	IDDatabase::IDDatabase()
	{
		load();
	}

	void IDDatabase::load()
	{
		auto&             mod = Module::get();
		const auto        dist = mod.distribution();
		const std::string build{ mod.build_code() };

		// Abstract ids are version-neutral: every distribution -- including Steam --
		// remaps through its own generated table (no privileged "identity" build).
		const char*   dn = nullptr;
		std::uint32_t expectDist = 0;
		switch (dist) {
		case Distribution::Steam: dn = "steam"; expectDist = 1; break;
		case Distribution::GOG:   dn = "gog";   expectDist = 2; break;
		case Distribution::Epic:  dn = "epic";  expectDist = 3; break;
		default:
			fail("Could not detect the game distribution (Steam / GOG / Epic) from "
				 "WHGame.dll.\nKCSE cannot select an address library.");
		}

		std::string key = build.empty() ? std::string{ mod.release() } : build;  // build_code, else release
		if (key.empty()) {
			fail("Could not determine the game build/version.\nKCSE cannot select an address library.");
		}

		const std::wstring dir = s_dbDirSet ? s_dbDir : (mod.game_root() + L"\\KCSE\\addresslib");
		std::string  nameA = std::string("kcd_addresslib_") + dn + "_" + key + ".bin";
		std::wstring full = dir + L"\\" + std::wstring(nameA.begin(), nameA.end());

		std::ifstream f(full.c_str(), std::ios::binary);
		if (!f) {
			std::string p(full.size(), '\0');
			for (std::size_t i = 0; i < full.size(); ++i) {
				p[i] = static_cast<char>(full[i]);  // ASCII path display in the error box
			}
			fail("Address library not found:\n" + p +
				 "\n\nThis KCSE build has no address mapping for this game version/distribution.");
		}

		char magic[4]{};
		f.read(magic, 4);
		if (std::memcmp(magic, "KASL", 4) != 0) {
			fail("Address library has a bad magic header.");
		}

		std::uint32_t fmt = 0, fdist = 0, count = 0;
		f.read(reinterpret_cast<char*>(&fmt), 4);
		f.read(reinterpret_cast<char*>(&fdist), 4);
		f.read(reinterpret_cast<char*>(&count), 4);
		if (!f || count == 0) {
			fail("Address library header is invalid.");
		}
		if (fdist != expectDist) {
			fail("Address library is for a different distribution than the running game.");
		}

		_id2offset.resize(count);
		f.read(reinterpret_cast<char*>(_id2offset.data()),
			static_cast<std::streamsize>(count) * sizeof(mapping_t));
		if (!f) {
			fail("Address library is truncated.");
		}

		if (!std::is_sorted(_id2offset.begin(), _id2offset.end(),
				[](const mapping_t& a, const mapping_t& b) { return a.id < b.id; })) {
			std::sort(_id2offset.begin(), _id2offset.end(),
				[](const mapping_t& a, const mapping_t& b) { return a.id < b.id; });
		}
	}

	std::size_t IDDatabase::id2offset(std::uint64_t a_id) const
	{
		const mapping_t key{ static_cast<std::uint32_t>(a_id), 0 };
		const auto      it = std::lower_bound(
            _id2offset.begin(), _id2offset.end(), key,
            [](const mapping_t& a, const mapping_t& b) { return a.id < b.id; });
		if (it == _id2offset.end() || it->id != key.id) {
			char buf[160];
			std::snprintf(buf, sizeof(buf),
				"REL::ID %llu is not present in the address library for this version.\n"
				"This plugin is incompatible with the current game build.",
				static_cast<unsigned long long>(a_id));
			fail(buf);
		}
		return static_cast<std::size_t>(it->offset);
	}
}
