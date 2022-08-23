#include "scriptmanager.h"

#include "gold/graphics/model/primitives/factory.h"
#include "gold/input.h"
#include "gold/util/file.h"
#include "gold/util/time.h"

#define LOG_IF_ERROR(lua, error, prefix, filename)                                 \
	do                                                                             \
	{                                                                              \
		if (error)                                                                 \
		{                                                                          \
			LOG_ERROR(prefix << " " << filename << ": " << lua_tostring(lua, -1)); \
			lua_pop(lua, 1);                                                       \
		}                                                                          \
	} while (0);

static int lua_print(lua_State *lua)
{
	auto str = luaL_checklstring(lua, 1, NULL);
	LOG(str);

	return 0;
}

static int lua_get_object_pos(lua_State *lua)
{
	auto name   = luaL_checklstring(lua, 1, NULL);

	auto object = gold_factory.get_object(name);
	if (!object)
		return 0;

	const auto &pos = object->get_model()->get_pos();
	lua_pushnumber(lua, pos.x);
	lua_pushnumber(lua, pos.y);
	lua_pushnumber(lua, pos.z);

	return 3;
}

static int lua_set_object_pos(lua_State *lua)
{
	auto name   = luaL_checklstring(lua, 1, NULL);
	float x     = luaL_checknumber(lua, 2);
	float y     = luaL_checknumber(lua, 3);
	float z     = luaL_checknumber(lua, 4);

	auto object = gold_factory.get_object(name);
	if (object)
		object->get_model()->set_pos({ x, y, z });

	return 0;
}

static int lua_get_object_rot(lua_State *lua)
{
	auto name   = luaL_checklstring(lua, 1, NULL);

	auto object = gold_factory.get_object(name);
	if (!object)
		return 0;

	const auto &pos = object->get_model()->get_rotation();
	lua_pushnumber(lua, pos.x);
	lua_pushnumber(lua, pos.y);
	lua_pushnumber(lua, pos.z);

	return 3;
}

static int lua_get_object_angle(lua_State *lua)
{
	auto name   = luaL_checklstring(lua, 1, NULL);

	auto object = gold_factory.get_object(name);
	if (!object)
		return 0;

	lua_pushnumber(lua, object->get_model()->get_angle());

	return 1;
}

static int lua_set_object_rot(lua_State *lua)
{
	auto name   = luaL_checklstring(lua, 1, NULL);
	float angle = luaL_checknumber(lua, 2);
	float x     = luaL_checknumber(lua, 3);
	float y     = luaL_checknumber(lua, 4);
	float z     = luaL_checknumber(lua, 5);

	auto object = gold_factory.get_object(name);
	if (object)
		object->get_model()->set_rotation(angle, { x, y, z });

	return 0;
}

static int lua_key_pressed(lua_State *lua)
{
	auto key = luaL_checkinteger(lua, 1);

	lua_pushboolean(lua, gold_input.is_key_pressed(key));

	return 1;
}

static int lua_key_just_pressed(lua_State *lua)
{
	auto key = luaL_checkinteger(lua, 1);

	lua_pushboolean(lua, gold_input.is_key_just_pressed(key));

	return 1;
}

static void register_functions(lua_State *lua, const gold_string &filename)
{
	lua_pushcfunction(lua, lua_print);
	lua_setglobal(lua, "print");

	lua_pushcfunction(lua, lua_get_object_pos);
	lua_setglobal(lua, "get_object_position");

	lua_pushcfunction(lua, lua_set_object_pos);
	lua_setglobal(lua, "set_object_position");

	lua_pushcfunction(lua, lua_get_object_rot);
	lua_setglobal(lua, "get_object_rotation");

	lua_pushcfunction(lua, lua_get_object_angle);
	lua_setglobal(lua, "get_object_angle");

	lua_pushcfunction(lua, lua_set_object_rot);
	lua_setglobal(lua, "set_object_rotation");

	lua_pushcfunction(lua, lua_key_pressed);
	lua_setglobal(lua, "is_key_pressed");

	lua_pushcfunction(lua, lua_key_just_pressed);
	lua_setglobal(lua, "is_key_just_pressed");
}

void scriptmanager::register_script(std::string_view filename)
{
	for (const auto &script : script_states)
	{
		if (script == filename)
		{
			LOG_ERROR("Trying to register duplicate script " << filename << "!");
			return;
		}
	}

	if (!gold_file::does_file_exist(filename))
	{
		LOG_ERROR("Script " << filename << " not found!");
		return;
	}

	LOG("Registering script " << filename);

	auto file = gold_file::open_file(filename.data(), "r");
	fseek(file.handle, 0, SEEK_END);
	auto file_size = ftell(file.handle);
	rewind(file.handle);

	std::string buffer;
	buffer.resize(file_size);
	fread(buffer.data(), sizeof(char), file_size, file.handle);

	auto lua = luaL_newstate();
	luaL_openlibs(lua);

	auto error = luaL_loadbuffer(lua, buffer.data(), file_size, filename.data());
	LOG_IF_ERROR(lua, error, "Error while loading", filename);
	if (error)
		return;

	register_functions(lua, filename);

	error = lua_pcall(lua, 0, 0, 0);
	LOG_IF_ERROR(lua, error, "Error while executing", filename);
	if (error)
		return;

	script_states.push(script { .script_name = std::string(filename), .lua = lua });
}

void scriptmanager::unregister_script(std::string_view filename)
{
	for (auto it = script_states.begin(); it != script_states.end();)
	{
		if (*it == filename)
		{
			LOG("Unregistered script " << filename);
			it = script_states.erase_element(it);
			return;
		}
		else
			it++;
	}

	LOG_ERROR("Couldn't unregister script " << filename << ": not found!");
}

void scriptmanager::execute_all_scripts()
{
	for (auto &script : script_states)
	{
		const auto &lua = script.lua;
		lua_getglobal(lua, "on_tick");
		lua_pushnumber(lua, gold_delta_time);
		auto error = lua_pcall(lua, 1, 0, 0);
		LOG_IF_ERROR(lua, error, "Error during on_tick of", script.script_name);
	}
}

void scriptmanager::execute_script(std::string_view filename)
{
	const script *found_script = nullptr;
	for (const auto &script : script_states)
	{
		if (script == filename)
		{
			found_script = &script;
			break;
		}
	}

	if (!found_script)
	{
		LOG_ERROR("Couldn't execute script " << filename << ": not found!");
		return;
	}

	const auto &lua = found_script->lua;
	lua_getglobal(lua, "on_tick");
	lua_pushnumber(lua, gold_delta_time);
	auto error = lua_pcall(lua, 1, 0, 0);
	LOG_IF_ERROR(lua, error, "Error during on_tick of", found_script->script_name);
}