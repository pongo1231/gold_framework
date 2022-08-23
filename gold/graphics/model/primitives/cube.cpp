#include "cube.h"

#include "gold/camera.h"
#include "gold/graphics/mesh.h"
#include "gold/graphics/shader.h"
#include "gold/graphics/shader_program.h"
#include "gold/graphics/texture.h"
#include "gold/util/macros.h"
#include "gold/util/vertex.h"

#include <GL/glew.h>

static const gold_vector<gold_vertex> cube_vertices = {
	{ -1.f, 1.f, -1.f, 0.f, 1.f, 0.f, 0.4f, 0.5f, 1.f, 0.f, 1.f, 0.f },
	{ 1.f, 1.f, -1.f, 1.f, 1.f, 0.f, 0.4f, 0.5f, 1.f, 0.f, 1.f, 0.f },
	{ -1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.4f, 0.5f, 1.f, 0.f, 1.f, 0.f },
	{ 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.4f, 0.1f, 1.f, 0.f, 1.f, 0.f },

	{ -1.f, -1.f, -1.f, 1.f, 0.f, 0.1f, 0.4f, 0.f, 1.f, 0.f, -1.f, 0.f },
	{ 1.f, -1.f, -1.f, 0.f, 0.f, 0.5f, 0.4f, 0.f, 1.f, 0.f, -1.f, 0.f },
	{ -1.f, -1.f, 1.f, 1.f, 1.f, 0.5f, 0.4f, 0.f, 1.f, 0.f, -1.f, 0.f },
	{ 1.f, -1.f, 1.f, 0.f, 1.f, 0.5f, 0.4f, 0.f, 1.f, 0.f, -1.f, 0.f },

	{ 1.f, 1.f, 1.f, 0.f, 1.f, 0.f, 0.4f, 0.f, 1.f, 1.f, 0.f, 0.f },
	{ 1.f, 1.f, -1.f, 1.f, 1.f, 0.f, 0.4f, 0.f, 1.f, 1.f, 0.f, 0.f },
	{ 1.f, -1.f, 1.f, 0.f, 0.f, 0.f, 0.4f, 0.f, 1.f, 1.f, 0.f, 0.f },
	{ 1.f, -1.f, -1.f, 0.f, 1.f, 0.f, 0.4f, 0.f, 1.f, 1.f, 0.f, 0.f },

	{ -1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.4f, 0.f, 1.f, -1.f, 0.f, 0.f },
	{ -1.f, 1.f, -1.f, 0.f, 0.f, 0.f, 0.4f, 0.f, 1.f, -1.f, 0.f, 0.f },
	{ -1.f, -1.f, 1.f, 0.f, 0.f, 0.f, 0.4f, 0.f, 1.f, -1.f, 0.f, 0.f },
	{ -1.f, -1.f, -1.f, 1.f, 0.f, 0.f, 0.4f, 0.f, 1.f, -1.f, 0.f, 0.f },

	{ -1.f, 1.f, 1.f, 0.f, 1.f, 0.f, 0.4f, 0.4f, 1.f, 0.f, 0.f, 1.f },
	{ 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.4f, 0.4f, 1.f, 0.f, 0.f, 1.f },
	{ -1.f, -1.f, 1.f, 0.f, 0.f, 0.f, 0.4f, 0.4f, 1.f, 0.f, 0.f, 1.f },
	{ 1.f, -1.f, 1.f, 1.f, 0.f, 0.f, 0.4f, 0.4f, 1.f, 0.f, 0.f, 1.f },

	{ -1.f, 1.f, -1.f, 1.f, 0.f, 0.4f, 0.4f, 0.f, 1.f, 0.f, 0.f, -1.f },
	{ 1.f, 1.f, -1.f, 0.f, 0.f, 0.4f, 0.4f, 0.f, 1.f, 0.f, 0.f, -1.f },
	{ -1.f, -1.f, -1.f, 1.f, 1.f, 0.4f, 0.4f, 0.f, 1.f, 0.f, 0.f, -1.f },
	{ 1.f, -1.f, -1.f, 0.f, 1.f, 0.4f, 0.4f, 0.f, 1.f, 0.f, 0.f, -1.f },
};

static const gold_vector<std::uint32_t> cube_indices = { 8,  9,  10, 9,  11, 10,

	                                                     14, 13, 12, 14, 15, 13,

	                                                     1,  2,  0,  3,  2,  1,

	                                                     4,  6,  5,  5,  6,  7,

	                                                     17, 18, 16, 19, 18, 17,

	                                                     20, 22, 21, 21, 22, 23 };

gold_cube::gold_cube(const gold_vector3 &pos, bool is_plane)
{
	auto cube_mesh = gold_unique_ptr<gold_mesh>::create(cube_vertices, cube_indices);
	cube_mesh->set_triangle_strip(true);
	auto cube_vert_shader = gold_shader::load_from_file("shaders/cube_vert.glsl", GL_VERTEX_SHADER);
	gold_ref_ptr<gold_shader> cube_frag_shader;
	if (is_plane)
		cube_frag_shader = gold_shader::load_from_file("shaders/plane_frag.glsl", GL_FRAGMENT_SHADER);
	else
		cube_frag_shader = gold_shader::load_from_file("shaders/cube_frag.glsl", GL_FRAGMENT_SHADER);
	auto cube_shader_program = gold_unique_ptr<gold_shader_program>::create(cube_vert_shader, cube_frag_shader);

	cube_shader_program->bind();
	cube_shader_program->set_uniform_vector3("uni_light_pos", { 0.f, 0.f, 0.f });
	cube_shader_program->set_uniform_vector3("uni_light_col", { .0f, .5f, .5f });
	cube_shader_program->set_uniform_float("uni_ambient_modifier", .9f);
	cube_shader_program->set_uniform_float("uni_spec_modifier", specular_multiplier);
	cube_shader_program->set_uniform_float("uni_shininess", shininess);
	cube_shader_program->unbind();

	model = gold_unique_ptr<gold_model>::create(cube_mesh, cube_shader_program);
	// mod->set_tex(new gol_tex("imgs/tex.png"));
	model->set_pos(pos);
}

void gold_cube::render(const gold_camera *camera) const
{
	glEnable(GL_DEPTH_TEST);

	model->get_shader_program()->bind();
	model->get_shader_program()->set_uniform_vector3("uni_cam_pos", camera->get_eye());

	model->render(camera);
}

gold_model *gold_cube::get_model() const
{
	return model.handle();
}

void gold_cube::set_specular_multiplier(float spec_multiplier)
{
	specular_multiplier = spec_multiplier;

	model->get_shader_program()->bind();
	model->get_shader_program()->set_uniform_float("uni_spec_modifier", specular_multiplier);
	model->get_shader_program()->unbind();
}

void gold_cube::set_shininess(float _shininess)
{
	shininess = _shininess;

	model->get_shader_program()->bind();
	model->get_shader_program()->set_uniform_float("uni_shininess", shininess);
	model->get_shader_program()->unbind();
}