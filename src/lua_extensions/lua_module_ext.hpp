#pragma once

#include "lua/lua_module.hpp"

namespace big
{
	struct lua_module_data_ext
	{
		std::vector<sol::protected_function> m_on_pre_import;
		std::vector<sol::protected_function> m_on_post_import;

		std::vector<sol::protected_function> m_on_xml_parse;

		std::vector<sol::protected_function> m_on_pak_openable;

		ankerl::unordered_dense::map<std::string, std::string> m_cryfile_open_replace_map;

		std::vector<sol::protected_function> m_on_fmod_getevent;

		std::vector<sol::protected_function> m_on_cryengine_lua_userdata_bind;
	};

	class lua_module_ext : public lua_module
	{
	public:
		lua_module_data_ext m_data_ext;

	public:

		lua_module_ext(const module_info& module_info, sol::environment& env) :
		    lua_module(module_info, env)
		{
		}

		lua_module_ext(const module_info& module_info, sol::state_view& state) :
		    lua_module(module_info, state)
		{
		}

		inline void cleanup() override
		{
			lua_module::cleanup();

			m_data_ext = {};
		}
	};
} // namespace big
