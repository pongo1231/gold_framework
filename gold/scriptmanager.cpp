#include "scriptmanager.h"

#include "gold/graphics/model/factory.h"
#include "gold/input.h"
#include "gold/util/file.h"
#include "gold/util/string.h"
#include "gold/util/time.h"

#include <filesystem>

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

static int lua_create_entity(lua_State *lua)
{
	auto name   = luaL_checklstring(lua, 1, NULL);
	auto filename   = luaL_checklstring(lua, 2, NULL);

	if (!gold_factory.create_entity<gold_model_type::obj_model>(name, filename))
		return 0;

	lua_pushstring(lua, name);

	return 1;
}

static int lua_create_cube(lua_State *lua)
{
	auto name     = luaL_checklstring(lua, 1, NULL);

	if (!gold_factory.create_entity<gold_model_type::cube>(name))
		return 0;

	lua_pushstring(lua, name);

	return 1;
}

static int lua_create_skybox(lua_State *lua)
{
	auto name = luaL_checklstring(lua, 1, NULL);

	if (!gold_factory.create_entity<gold_model_type::skybox>(name))
		return 0;

	lua_pushstring(lua, name);

	return 1;
}

static int lua_get_entity_position(lua_State *lua)
{
	auto name   = luaL_checklstring(lua, 1, NULL);

	auto entity = gold_factory.get_entity(name);
	if (!entity)
		return 0;

	const auto &pos = entity->get_position();
	lua_pushnumber(lua, pos.x);
	lua_pushnumber(lua, pos.y);
	lua_pushnumber(lua, pos.z);

	return 3;
}

static int lua_set_entity_position(lua_State *lua)
{
	auto name   = luaL_checklstring(lua, 1, NULL);
	float x     = luaL_checknumber(lua, 2);
	float y     = luaL_checknumber(lua, 3);
	float z     = luaL_checknumber(lua, 4);

	auto entity = gold_factory.get_entity(name);
	if (entity)
		entity->set_position({ x, y, z });

	return 0;
}

static int lua_get_entity_rotation(lua_State *lua)
{
	auto name   = luaL_checklstring(lua, 1, NULL);

	auto entity = gold_factory.get_entity(name);
	if (!entity)
		return 0;

	const auto &pos = entity->get_rotation();
	lua_pushnumber(lua, pos.x);
	lua_pushnumber(lua, pos.y);
	lua_pushnumber(lua, pos.z);

	return 3;
}

static int lua_set_entity_rotation(lua_State *lua)
{
	auto name   = luaL_checklstring(lua, 1, NULL);
	float x     = luaL_checknumber(lua, 2);
	float y     = luaL_checknumber(lua, 3);
	float z     = luaL_checknumber(lua, 4);

	auto entity = gold_factory.get_entity(name);
	if (entity)
		entity->set_rotation({ x, y, z });

	return 0;
}

static int lua_entity_move(lua_State *lua)
{
	auto name   = luaL_checklstring(lua, 1, NULL);
	float x     = luaL_checknumber(lua, 2);
	float y     = luaL_checknumber(lua, 3);
	float z     = luaL_checknumber(lua, 4);

	auto entity = gold_factory.get_entity(name);
	if (entity)
		entity->move({ x, y, z });

	return 0;
}

static int lua_entity_move_relative(lua_State *lua)
{
	auto name   = luaL_checklstring(lua, 1, NULL);
	float x     = luaL_checknumber(lua, 2);
	float y     = luaL_checknumber(lua, 3);
	float z     = luaL_checknumber(lua, 4);

	auto entity = gold_factory.get_entity(name);
	if (entity)
		entity->move_relative({ x, y, z });

	return 0;
}

static int lua_entity_add_velocity(lua_State *lua)
{
	auto name   = luaL_checklstring(lua, 1, NULL);
	float x     = luaL_checknumber(lua, 2);
	float y     = luaL_checknumber(lua, 3);
	float z     = luaL_checknumber(lua, 4);

	auto entity = gold_factory.get_entity(name);
	if (entity)
		entity->add_velocity({ x, y, z });

	return 0;
}

static int lua_entity_add_velocity_relative(lua_State *lua)
{
	auto name   = luaL_checklstring(lua, 1, NULL);
	float x     = luaL_checknumber(lua, 2);
	float y     = luaL_checknumber(lua, 3);
	float z     = luaL_checknumber(lua, 4);

	auto entity = gold_factory.get_entity(name);
	if (entity)
		entity->add_velocity_relative({ x, y, z });

	return 0;
}

static int lua_entity_reset_velocity(lua_State *lua)
{
	auto name   = luaL_checklstring(lua, 1, NULL);

	auto entity = gold_factory.get_entity(name);
	if (entity)
		entity->reset_velocity();

	return 0;
}

static int lua_get_entity_velocity(lua_State *lua)
{
	auto name   = luaL_checklstring(lua, 1, NULL);

	auto entity = gold_factory.get_entity(name);
	if (!entity)
		return 0;

	const auto &pos = entity->get_velocity();
	lua_pushnumber(lua, pos.x);
	lua_pushnumber(lua, pos.y);
	lua_pushnumber(lua, pos.z);

	return 3;
}

static int lua_get_entity_up(lua_State *lua)
{
	auto name   = luaL_checklstring(lua, 1, NULL);

	auto entity = gold_factory.get_entity(name);
	if (!entity)
		return 0;

	const auto &pos = entity->get_up();
	lua_pushnumber(lua, pos.x);
	lua_pushnumber(lua, pos.y);
	lua_pushnumber(lua, pos.z);

	return 3;
}

static int lua_get_entity_forward(lua_State *lua)
{
	auto name   = luaL_checklstring(lua, 1, NULL);

	auto entity = gold_factory.get_entity(name);
	if (!entity)
		return 0;

	const auto &pos = entity->get_forward();
	lua_pushnumber(lua, pos.x);
	lua_pushnumber(lua, pos.y);
	lua_pushnumber(lua, pos.z);

	return 3;
}

static int lua_get_entity_left(lua_State *lua)
{
	auto name   = luaL_checklstring(lua, 1, NULL);

	auto entity = gold_factory.get_entity(name);
	if (!entity)
		return 0;

	const auto &pos = entity->get_left();
	lua_pushnumber(lua, pos.x);
	lua_pushnumber(lua, pos.y);
	lua_pushnumber(lua, pos.z);

	return 3;
}

static int lua_is_entity_colliding_with(lua_State *lua)
{
	auto name   = luaL_checklstring(lua, 1, NULL);
	auto name2   = luaL_checklstring(lua, 2, NULL);

	auto entity = gold_factory.get_entity(name);
	if (!entity)
		return 0;

	auto entity2 = gold_factory.get_entity(name2);
	if (!entity2)
		return 0;

	bool is_colliding = entity->is_colliding_with(*entity2);
	lua_pushboolean(lua, is_colliding);

	return 1;
}

static int lua_is_entity_colliding_with_anything(lua_State *lua)
{
	auto name   = luaL_checklstring(lua, 1, NULL);

	auto entity = gold_factory.get_entity(name);
	if (!entity)
		return 0;

	bool is_colliding = entity->is_colliding_with_anything();
	lua_pushboolean(lua, is_colliding);

	return 1;
}

static int lua_is_entity_colliding_with_any_trigger(lua_State *lua)
{
	auto name   = luaL_checklstring(lua, 1, NULL);

	auto entity = gold_factory.get_entity(name);
	if (!entity)
		return 0;

	bool is_colliding = entity->is_colliding_with_any_trigger();
	lua_pushboolean(lua, is_colliding);

	return 1;
}

static int lua_is_entity_on_ground(lua_State *lua)
{
	auto name   = luaL_checklstring(lua, 1, NULL);

	auto entity = gold_factory.get_entity(name);
	if (!entity)
		return 0;

	bool on_ground = entity->is_on_ground();
	lua_pushboolean(lua, on_ground);

	return 1;
}

static int lua_does_entity_exist(lua_State *lua)
{
	auto name   = luaL_checklstring(lua, 1, NULL);

	auto entity = gold_factory.get_entity(name);
	lua_pushboolean(lua, entity != nullptr);

	return 1;
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

static void register_base_functions(lua_State *lua, std::string_view filename)
{
	lua_pushcfunction(lua, lua_print);
	lua_setglobal(lua, "print");

	lua_pushcfunction(lua, lua_create_entity);
	lua_setglobal(lua, "create_entity");

	lua_pushcfunction(lua, lua_create_cube);
	lua_setglobal(lua, "create_cube");

	lua_pushcfunction(lua, lua_create_skybox);
	lua_setglobal(lua, "create_skybox");

	lua_pushcfunction(lua, lua_get_entity_position);
	lua_setglobal(lua, "get_entity_position");

	lua_pushcfunction(lua, lua_set_entity_position);
	lua_setglobal(lua, "set_entity_position");

	lua_pushcfunction(lua, lua_get_entity_rotation);
	lua_setglobal(lua, "get_entity_rotation");

	lua_pushcfunction(lua, lua_set_entity_rotation);
	lua_setglobal(lua, "set_entity_rotation");

	lua_pushcfunction(lua, lua_entity_move);
	lua_setglobal(lua, "move_entity");

	lua_pushcfunction(lua, lua_entity_move_relative);
	lua_setglobal(lua, "move_entity_relative");

	lua_pushcfunction(lua, lua_entity_add_velocity);
	lua_setglobal(lua, "add_entity_velocity");

	lua_pushcfunction(lua, lua_entity_add_velocity_relative);
	lua_setglobal(lua, "add_entity_velocity_relative");

	lua_pushcfunction(lua, lua_entity_reset_velocity);
	lua_setglobal(lua, "reset_entity_velocity");

	lua_pushcfunction(lua, lua_get_entity_velocity);
	lua_setglobal(lua, "get_entity_velocity");

	lua_pushcfunction(lua, lua_get_entity_up);
	lua_setglobal(lua, "get_entity_up");

	lua_pushcfunction(lua, lua_get_entity_forward);
	lua_setglobal(lua, "get_entity_forward");

	lua_pushcfunction(lua, lua_get_entity_up);
	lua_setglobal(lua, "get_entity_up");

	lua_pushcfunction(lua, lua_is_entity_colliding_with);
	lua_setglobal(lua, "is_entity_colliding_with");

	lua_pushcfunction(lua, lua_is_entity_colliding_with_anything);
	lua_setglobal(lua, "is_entity_colliding_with_anything");

	lua_pushcfunction(lua, lua_is_entity_colliding_with_any_trigger);
	lua_setglobal(lua, "is_entity_colliding_with_any_trigger");

	lua_pushcfunction(lua, lua_is_entity_on_ground);
	lua_setglobal(lua, "is_entity_on_ground");

	lua_pushcfunction(lua, lua_does_entity_exist);
	lua_setglobal(lua, "does_entity_exist");

	lua_pushcfunction(lua, lua_key_pressed);
	lua_setglobal(lua, "is_key_pressed");

	lua_pushcfunction(lua, lua_key_just_pressed);
	lua_setglobal(lua, "is_key_just_pressed");
}

void gold_scriptmanager::register_script(std::string_view filename)
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

	gold_string buffer;
	buffer.resize(file_size);
	fread(buffer.c_string(), sizeof(char), file_size, file.handle);

	auto lua = luaL_newstate();
	luaL_openlibs(lua);

	auto error = luaL_loadbuffer(lua, buffer.c_string(), file_size, filename.data());
	LOG_IF_ERROR(lua, error, "Error while loading", filename);
	if (error)
		return;

	register_base_functions(lua, filename);

	for (const auto &custom_function : custom_functions)
	{
		lua_pushcfunction(lua, custom_function.function);
		lua_setglobal(lua, custom_function.function_name.c_string());
	}

	error = lua_pcall(lua, 0, 0, 0);
	LOG_IF_ERROR(lua, error, "Error while executing", filename);
	if (error)
		return;

	script_states.push(script { .script_name = gold_string(filename.data()), .lua = lua });
}

void gold_scriptmanager::register_scripts(std::string_view path)
{
	for (const auto &entry : std::filesystem::directory_iterator(path))
	{
		if (entry.is_regular_file() && entry.path().has_extension() && entry.path().extension() == ".lua"
		    && entry.file_size() > 0)
			register_script(entry.path().string());
	}
}

void gold_scriptmanager::unregister_script(std::string_view filename)
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

void gold_scriptmanager::unregister_all_scripts()
{
	for (const auto& script : script_states)
		LOG("Unregistered script " << script.script_name);

	script_states.clear();
}

void gold_scriptmanager::execute_all_scripts()
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

void gold_scriptmanager::execute_script(std::string_view filename)
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

const gold_vector<gold_scriptmanager::script> &gold_scriptmanager::get_all_scripts() const
{
	return script_states;
}

void gold_scriptmanager::add_function(const gold_scriptmanager::custom_function &custom_function)
{
	if (!custom_function.function || custom_function.function_name.is_empty())
		return;

	custom_functions.push(custom_function);

	for (const auto& script : script_states)
	{
		lua_pushcfunction(script.lua, custom_function.function);
		lua_setglobal(script.lua, custom_function.function_name.c_string());
	}
}