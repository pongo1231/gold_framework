#pragma once

#include "gold/util/macros.h"
#include "gold/util/util.h"

extern "C"
{
#include <lua/lauxlib.h>
#include <lua/lua.h>
#include <lua/lualib.h>
}

#include <memory>
#include <string>
#include <string_view>
#include <vector>

class gold_input;

class scriptmanager
{
	struct script
	{
		std::string script_name;
		lua_State *lua;

		_NODISCARD bool operator==(std::string_view script_name) const
		{
			return this->script_name == script_name;
		}
	};

	std::vector<script> script_states;

  public:
	void register_script(std::string_view filename);
	void unregister_script(std::string_view filename);

	void execute_all_scripts();
	void execute_script(std::string_view filename);
};

inline scriptmanager scriptmanager;