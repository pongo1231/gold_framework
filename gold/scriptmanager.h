#pragma once

#include "gold/memory.h"
#include "gold/util/macros.h"
#include "gold/util/string.h"
#include "gold/util/util.h"

extern "C"
{
#include <lua/lauxlib.h>
#include <lua/lua.h>
#include <lua/lualib.h>
}

#include <string_view>

class gold_scriptmanager
{
	struct script
	{
		gold_string script_name;
		lua_State *lua;

		bool operator==(std::string_view script_name) const
		{
			return this->script_name == script_name;
		}
	};

	gold_vector<script> script_states;

	struct custom_function
	{
		gold_string function_name;
		int(*function)(lua_State*) = nullptr;
	};

	gold_vector<custom_function> custom_functions;

  public:
	void register_script(std::string_view filename);
	void register_scripts(std::string_view path);
	void unregister_script(std::string_view filename);
	void unregister_all_scripts();

	void execute_all_scripts();
	void execute_script(std::string_view filename);

	const gold_vector<script> &get_all_scripts() const;

	void add_function(const gold_scriptmanager::custom_function &custom_function);
};

inline gold_scriptmanager script_manager;