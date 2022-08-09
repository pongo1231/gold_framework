#include "game.h"

#include "gold/error_code.h"
#include "gold/graphics/mesh.h"
#include "gold/graphics/model/primitives/factory.h"
#include "gold/graphics/shader.h"
#include "gold/graphics/shader_program.h"
#include "gold/memory.h"
#include "gold/scriptmanager.h"
#include "gold/util/macros.h"
#include "gold/util/vector.h"
#include "gold/util/vertex.h"

#define WIN32_LEAN_AND_MEAN
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <windows.h>

#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

static std::vector<std::string> demo_scripts = { "scripts/pingpong.lua", "scripts/circle.lua", "scripts/random.lua",
	                                             "scripts/playerinput.lua" };
static size_t demo_current_script_index      = 0;

error_code gold_game::init(HINSTANCE inst)
{
	if (did_init)
		return error_code::already_run;

	graphics_device = std::make_unique<gold_graphicsdevice>();

	camera          = std::make_unique<gold_camera>(graphics_device.get());
	camera->set_eye({ 0.f, 30.f, -15.f });
	camera->set_look_at({ 0.f, 0.f, 0.f });
	camera->set_up({ 0.f, 1.f, 0.f });
	camera->set_fov(45.f, 1280.f / 720.f);

	error_code error_code = error_code::success;

	if ((error_code = graphics_device->init(inst)) != error_code::success)
		return error_code;

	plane = gold_factory.create_cube({ 0.f, -2.f, 0.f }, "plane", true);
	plane->get_model()->set_scale({ 10.f, 1.f, 10.f });
	plane->set_specular_multiplier(0.f);
	plane->set_shininess(0.f);

	cube = gold_factory.create_cube({ 0.f, 0.f, -3.f }, "cube");
	cube->set_specular_multiplier(1.f);
	cube->set_shininess(10.f);

	// skybox = std::make_unique<gold_skybox>();

	for (const auto &script_name : demo_scripts)
		scriptmanager.register_script(script_name);

	LOG("hi");
	gold_vector<int> test;
	LOG("whaddup");
	test.push(5);
	LOG("nothing");
	test.push(7);
	LOG("lol");
	test.push(9);
	for (const auto &element : test)
		LOG(element);
	LOG("");
	test.pop();
	for (const auto &element : test)
		LOG(element);
	LOG("");
	test.push(8888);
	test.push(666);
	for (const auto &element : test)
		LOG(element);
	LOG("");
	test.resize(2);
	for (const auto &element : test)
		LOG(element);

	return error_code::success;
}

error_code gold_game::run()
{
	gold_input.run();

	scriptmanager.execute_script(demo_scripts[demo_current_script_index]);

	/*if (gold_input.is_key_pressed(0x57)) // W
	    camera->move({ 0.f, 0.f, .1f });
	else if (gold_input.is_key_pressed(0x53)) // S
	    camera->move({ 0.f, 0.f, -.1f });

	if (gold_input.is_key_pressed(0x41)) // A
	    camera->move({ .1f, 0.f, 0.f });
	else if (gold_input.is_key_pressed(0x44)) // D
	    camera->move({ -.1f, 0.f, 0.f });*/

	if (gold_input.is_key_pressed(VK_CONTROL))
	{
		if (gold_input.is_key_just_pressed(0x47) /* G */)
		{
			if (++demo_current_script_index >= demo_scripts.size())
				demo_current_script_index = 0;
		}
		else if (gold_input.is_key_just_pressed(0x52) /* R */)
		{
			for (const auto &script_name : demo_scripts)
			{
				scriptmanager.unregister_script(script_name);
				scriptmanager.register_script(script_name);
			}
		}
	}

	error_code error_code = error_code::success;
	if ((error_code = graphics_device->begin_render()) != error_code::success)
		return error_code;

	camera->update();

	// skybox->render(camera.get());

	// auto the_sin    = 1 + sinf(GetTickCount64() * .001f) * .5f;
	// auto cube_scale = gold_vector3(2.f * the_sin, 2.f * the_sin, 2.f * the_sin);
	//  cube_1->get_model()->set_scale(gold_vector3(10.f, 10.f, 10.f));
	// cube->get_model()->set_rotation(GetTickCount64() * .01f * glm::radians(45.f), { 1.f, 1.f, 1.f }, true);
	//  cube_1->get_model()->rotate(GetTickCount64() * .001f * glm::radians(45.f), { 1.f, 1.f, 1.f }, true);
	//  cube_1->get_model()->set_pos(cube_1->get_model()->get_pos() + gold_vector3(0.f, 0.f, 0.01f));
	cube->render(camera.get());

	plane->render(camera.get());

	glUseProgram(0);

	/*const auto &pos = camera->get_eye();

	char buffer[128] {};
	sprintf_s(buffer, "Position - X: %f Y: %f Z: %f", pos.x, pos.y, pos.z);

	// Taken from https://stackoverflow.com/questions/6858524/convert-char-to-lpwstr
	wchar_t title[128] {};
	size_t conv_size = 0;
	mbstowcs_s(&conv_size, title, buffer, strlen(buffer) + 1);*/

	std::ostringstream oss;
	oss << "STRG + G - Skript wechseln / STRG + R - Skripte neu laden | Momentaner Skript: "
	    << demo_scripts[demo_current_script_index];

	auto str  = oss.str();
	auto wstr = std::wstring(str.begin(), str.end());

	SetWindowText(graphics_device->get_wnd(), wstr.c_str());

	if ((error_code = graphics_device->end_render()) != error_code::success)
		return error_code;

	return error_code::success;
}

_NODISCARD gold_camera *gold_game::get_camera() const
{
	return camera.get();
}