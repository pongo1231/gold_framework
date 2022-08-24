#include "game.h"

#include "gold/gold.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cstdio>
#include <fstream>
#include <sstream>

static gold_vector<gold_string> demo_scripts = { "scripts/pingpong.lua", "scripts/circle.lua", "scripts/random.lua",
	                                             "scripts/playerinput.lua" };
static size_t demo_current_script_index      = 0;

gold_ref_ptr<gold_texture> texture;

error_code gold_game::init(HINSTANCE inst)
{
	if (did_init)
		return error_code::already_run;

	graphics_device = gold_unique_ptr<gold_graphicsdevice>::create();

	camera          = gold_unique_ptr<gold_camera>::create(graphics_device.handle());
	camera->set_eye({ 0.f, 10.f, -20.f });
	camera->set_look_at({ 0.f, 0.f, 0.f });
	camera->set_up({ 0.f, 1.f, 0.f });
	camera->set_fov(45.f, 1280.f / 720.f);

	error_code error_code = error_code::success;

	if ((error_code = graphics_device->init(inst, 1920, 1080)) != error_code::success)
		return error_code;

	texture = gold_ref_ptr<gold_texture>::create("textures/wall.jpg");

	plane   = gold_factory.create_model<gold_model_type::cube, gold_cube>("plane");
	plane->set_position({ 0.f, -2.f, 0.f });
	plane->set_scale({ 10.f, 1.f, 10.f });
	plane->set_specular_multiplier(0.f);
	plane->set_shininess(0.f);
	plane->set_texture(texture);

	cube = gold_factory.create_model<gold_model_type::cube, gold_cube>("cube");
	cube->set_position({ 0.f, 0.f, -3.f });
	cube->set_specular_multiplier(1.f);
	cube->set_shininess(10.f);
	cube->set_texture(texture);

	skybox = gold_unique_ptr<gold_skybox>(gold_skybox::create());
	// gold_factory.create_model<gold_model_type::skybox, gold_skybox>("skybox");

	for (const auto &script_name : demo_scripts)
		scriptmanager.register_script(script_name);

	model = gold_model::load_from_obj("models/buildingno.obj");
	model->set_position({ 0.f, -10.f, 50.f });
	model->set_texture(texture);

	model2 = gold_model::load_from_obj("models/buildingno.obj");
	model2->set_position({ 30.f, 0.f, -30.f });

	return error_code::success;
}

error_code gold_game::run()
{
	gold_input.run();

	scriptmanager.execute_script(demo_scripts[demo_current_script_index]);

	const auto &camera_pos     = camera->get_eye();
	const auto &camera_look_at = camera->get_look_at();

	camera->set_eye({ std::max(-200.f, std::min(camera_pos.x, 200.f)), std::max(-200.f, std::min(camera_pos.y, 200.f)),
	                  std::max(-200.f, std::min(camera_pos.z, 200.f)) });

	gold_vector3 move;

	if (gold_input.is_key_pressed(0x57)) // W
		move.x -= .1f;
	if (gold_input.is_key_pressed(0x53)) // S
		move.x += .1f;

	if (gold_input.is_key_pressed(0x41)) // A
		move.z -= .1f;
	else if (gold_input.is_key_pressed(0x44)) // D
		move.z += .1f;

	camera->move_relative(move);

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

	/*const float radius          = 40.0f;
	static float rotation_thing = 0.f;
	rotation_thing += gold_delta_time * 0.5f;
	float cam_x = sin(rotation_thing) * radius;
	float cam_z = cos(rotation_thing) * radius;
	camera->set_look_at({ 0.f, 0.f, 0.f });
	camera->set_eye({ cam_x, 10.f, cam_z });*/

	const auto &cursor_dist = graphics_device->get_last_cursor_distance();

	static float yaw        = 0.f;
	static float pitch      = 0.f;
	yaw += cursor_dist.x * 0.05f;
	pitch = std::max(-89.f, std::min(pitch - cursor_dist.y * 0.05f, 89.f));

	gold_vector3 new_rot;
	new_rot.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	new_rot.y = sin(glm::radians(pitch));
	new_rot.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	// new_rot   = new_rot.norm();

	camera->set_look_at({ camera_pos.x + new_rot.x, camera_pos.y + new_rot.y, camera_pos.z + new_rot.z });

	camera->update();

	skybox->render(camera.handle());
	// gold_factory.get_object("skybox")->render(camera.handle());

	// auto the_sin    = 1 + sinf(GetTickCount64() * .001f) * .5f;
	// auto cube_scale = gold_vector3(2.f * the_sin, 2.f * the_sin, 2.f * the_sin);
	//  cube_1->get_model()->set_scale(gold_vector3(10.f, 10.f, 10.f));
	// cube->get_model()->set_rotation(GetTickCount64() * .01f * glm::radians(45.f), { 1.f, 1.f, 1.f }, true);
	//  cube_1->get_model()->rotate(GetTickCount64() * .001f * glm::radians(45.f), { 1.f, 1.f, 1.f }, true);
	//  cube_1->get_model()->set_pos(cube_1->get_model()->get_pos() + gold_vector3(0.f, 0.f, 0.01f));
	cube->render(camera.handle());

	plane->render(camera.handle());

	// model->set_position({ -5.f, -100.f, 100.f });
	auto &model_rot = model->get_rotation();
	model->set_rotation({ model_rot.x, model_rot.y + 1.f, model_rot.z });
	model->render(camera.handle());

	model2->render(camera.handle());

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
	    << demo_scripts[demo_current_script_index].c_string();

	auto str  = oss.str();
	auto wstr = std::wstring(str.begin(), str.end());

	SetWindowText(graphics_device->get_wnd(), wstr.c_str());

	if ((error_code = graphics_device->end_render()) != error_code::success)
		return error_code;

	return error_code::success;
}

gold_camera *gold_game::get_camera() const
{
	return camera.handle();
}