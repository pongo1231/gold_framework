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

/*
* Script manager
*/
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
	/*
	* Register lua script
	* <param name="filename">Path to file</param>
	*/
	void register_script(std::string_view filename);
	/*
	 * Register lua scripts inside a folder
	 * <param name="path">Path to folder</param>
	 */
	void register_scripts(std::string_view path);
	/*
	 * Unregister a registered script by path
	 * <param name="filename">Path of script to unregister</param>
	 */
	void unregister_script(std::string_view filename);
	/*
	 * Unregister all registered scripts
	 */
	void unregister_all_scripts();

	/*
	* Execute all registered scripts
	*/
	void execute_all_scripts();
	/*
	 * Execute a registered script
	 * <param name="filename">Path of registered script to execute</param>
	 */
	void execute_script(std::string_view filename);

	/*
	 * <returns>List of all registered scripts</returns>
	 */
	const gold_vector<script> &get_all_scripts() const;

	/*
	 * Register a custom function to scripting runtime
	 * <param name="custom_function">Custom function to register</param>
	 */
	void add_function(const gold_scriptmanager::custom_function &custom_function);
};

inline gold_scriptmanager script_manager;