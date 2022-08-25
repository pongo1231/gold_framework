#include "game.h"

#include <cstdio>
#include <fstream>
#include <sstream>

//static gold_vector<gold_string> demo_scripts = { "scripts/pingpong.lua", "scripts/circle.lua", "scripts/random.lua",
//	                                             "scripts/playerinput.lua" };
static size_t demo_current_script_index      = 0;

gold_ref_ptr<gold_texture> texture;

void gold_game::init(HINSTANCE inst)
{
	if (did_init)
		return;

	gold_init();

	graphics_device = gold_unique_ptr<gold_graphicsdevice>::create();

	camera          = gold_unique_ptr<gold_camera>::create(graphics_device.handle());
	camera->set_fov(45.f, 1280.f / 720.f);
	camera->rotatable     = true;

	graphics_device->init(inst, 1600, 900);

	texture = gold_ref_ptr<gold_texture>::create("textures/wall.jpg");

	start_platform   = gold_factory.create_entity<gold_model_type::cube>("start_platform");
	start_platform->set_position({ 0.f, -2.f, 0.f });
	start_platform->get_model()->set_scale({ 5.f, 1.f, 5.f });
	start_platform->get_model()->set_texture(texture);
	start_platform->has_gravity = false;

	cube = gold_factory.create_entity<gold_model_type::cube>("cube");
	cube->set_position({ 0.f, 0.f, 0.f });
	cube->get_model()->set_texture(texture);
	cube->has_gravity = false;

	player = gold_factory.create_entity<gold_model_type::cube>("player");
	player->set_position({ 0.f, 30.f, 1.f });
	camera->attach_to_entity(player);

	platform1 = gold_factory.create_entity<gold_model_type::cube>("platform1");
	platform1->set_position({ 0.f, 0.f, 20.f });
	platform1->get_model()->set_scale({ 5.f, 5.f, 5.f });
	platform1->get_model()->set_texture(texture);
	platform1->has_gravity = false;

	platform2              = gold_factory.create_entity<gold_model_type::obj_model>("platform2", "models/cube.obj");
	platform2->set_position({ 0.f, 4.f, 40.f });
	platform2->get_model()->set_scale({ 3.f, 3.f, 3.f });
	platform2->get_model()->set_texture(texture);
	platform2->has_gravity = false;

	platform3              = gold_factory.create_entity<gold_model_type::cube>("platform3");
	platform3->set_position({ 0.f, 8.f, 60.f });
	platform3->get_model()->set_scale({ 2.f, 2.f, 2.f });
	platform3->get_model()->set_texture(texture);
	platform3->has_gravity = false;

	skybox = gold_unique_ptr<gold_skybox>(gold_skybox::create());

	script_manager.register_scripts("scripts");

	model = gold_model::load_from_obj("models/building.obj");
	model->set_position({ 30.f, 0.f, 0.f });
	model->set_texture(texture);
	model->set_scale({ 2.f, 2.f, 2.f });

	model2 = gold_model::load_from_obj("models/building.obj");
	model2->set_position({ 30.f, 0.f, -30.f });
}

bool gold_game::run()
{
	if (!graphics_device->begin_render())
		return false;

	gold_input.run();

	script_manager.execute_script(script_manager.get_all_scripts()[demo_current_script_index].script_name);

	const auto &camera_pos     = camera->get_eye();
	const auto &camera_look_at = camera->get_look_at();

	static enum class camera_state
	{
		first_person,
		top_down
	} camera_state = camera_state::first_person;
	if (gold_input.is_key_just_pressed(VK_SHIFT))
	{
		if (camera_state == camera_state::first_person)
		{
			camera_state = camera_state::top_down;
			camera->rotatable = false;
			camera->detach_from_parent();
		}
		else
		{
			camera_state = camera_state::first_person;
			camera->rotatable = true;
			camera->set_eye({ 0.f, 0.f, 0.f });
			camera->set_look_at({ 0.f, 0.f, 0.f });
			camera->attach_to_entity(player);
		}
	}

	if (camera_state == camera_state::top_down)
	{
		const auto &player_pos = player->get_position();
		camera->set_eye({ player_pos.x, player_pos.y + 5.f, player_pos.z });
		camera->set_look_at({ player_pos.x, player_pos.y, player_pos.z });
	}

	gold_global_light_color = { .5f + std::sinf(GetTickCount64() * .0005f) * .5f, .5f + std::sinf(GetTickCount64() * .001f) * .5f,
		                        .5f + std::sinf(GetTickCount64() * .002f) * .5f };

	gold_vector3 move;

	if (gold_input.is_key_just_pressed(VK_SPACE) && player->is_on_ground())
		player->add_velocity({0.f, .1f, 0.f});

	if (gold_input.is_key_pressed(0x57)) // W
		move.x -= .1f;
	if (gold_input.is_key_pressed(0x53)) // S
		move.x += .1f;

	if (gold_input.is_key_pressed(0x41)) // A
		move.z -= .1f;
	else if (gold_input.is_key_pressed(0x44)) // D
		move.z += .1f;

	if (camera_state == camera_state::first_person)
		camera->move_parent_relatively(move, true);
	else
		player->move(move);

	if (gold_input.is_key_pressed(VK_CONTROL))
	{
		if (gold_input.is_key_just_pressed(0x47) /* G */)
		{
			if (++demo_current_script_index >= script_manager.get_all_scripts().size())
				demo_current_script_index = 0;
		}
		else if (gold_input.is_key_just_pressed(0x52) /* R */)
		{
			script_manager.unregister_all_scripts();
			script_manager.register_scripts("scripts");
		}
	}

	camera->update();

	skybox->render(camera.handle());
	model->render(camera.handle());
	model2->render(camera.handle());

	if (player->get_position().y < -10.f)
		player->set_position({ 0.f, 30.f, 1.f });

	for (const auto &[name, entity] : gold_factory.get_all_entities())
		entity->update(camera.handle());

	glUseProgram(0);

	std::ostringstream oss;
	oss << "STRG + G - Skript wechseln / STRG + R - Skripte neu laden | Momentaner Skript: "
	    << script_manager.get_all_scripts()[demo_current_script_index].script_name.c_string();

	auto str  = oss.str();

	graphics_device->set_window_title(str.c_str());

	graphics_device->end_render();

	return true;
}

gold_camera *gold_game::get_camera() const
{
	return camera.handle();
}