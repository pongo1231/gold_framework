#include "skybox.h"

#include "gold/camera.h"
#include "gold/graphics/mesh.h"
#include "gold/graphics/shader.h"
#include "gold/graphics/shader_program.h"
#include "gold/graphics/texture.h"
#include "gold/util/vector.h"
#include "gold/util/vertex.h"

#include <GL/glew.h>

#include <vector>

static const std::vector<gold_vertex> sky_verts = {
	{ -1.f, 1.f, -1.f, 0.f, 0.f, 0.5f, 0.f, 0.1f, 1.f }, // 0
	{ -1.f, -1.f, -1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f },  // 1
	{ 1.f, -1.f, -1.f, 1.f, 1.f, 0.f, 0.1f, 0.f, 1.f },  // 2

	{ -1.f, 1.f, -1.f, 1.f, 1.f, 0.5f, 0.f, 0.f, 1.f }, // 3
	{ -1.f, -1.f, 1.f, 1.f, 1.f, 0.f, 0.1f, 0.f, 1.f }, // 4
	{ 1.f, 1.f, -1.f, 1.f, 1.f, 0.5f, 0.f, 0.f, 1.f },  // 5

	{ -1.f, 1.f, 1.f, 1.f, 1.f, 0.5f, 0.1f, 0.f, 1.f }, // 6
	{ 1.f, -1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 0.1f, 1.f },  // 7
	{ 1.f, 1.f, 1.f, 1.f, 1.f, 0.5f, 0.f, 0.f, 1.f },   // 8
};

static const std::vector<unsigned short> sky_ids = { 0, 1, 2, 2, 5, 0,

	                                                 4, 1, 3, 3, 6, 4,

	                                                 2, 7, 8, 8, 5, 2,

	                                                 4, 6, 8, 8, 7, 4,

	                                                 0, 5, 8, 8, 6, 3,

	                                                 1, 4, 2, 2, 4, 7 };

gold_skybox::gold_skybox()
{
	auto mesh           = std::make_shared<gold_mesh>(sky_verts, sky_ids);
	auto vert_shader    = gold_shader::load_from_file("shaders/sky_vert.glsl", GL_VERTEX_SHADER);
	auto frag_shader    = gold_shader::load_from_file("shaders/sky_frag.glsl", GL_FRAGMENT_SHADER);
	auto shader_program = std::make_shared<gold_shader_program>(vert_shader, frag_shader);

	model               = std::make_unique<gold_model>(mesh, shader_program);
}

void gold_skybox::render(const gold_camera *camera) const
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);

	model->set_pos(camera->get_eye());
	model->render(camera);

	glDepthMask(GL_TRUE);
}

_NODISCARD gold_model *gold_skybox::get_model() const
{
	return model.get();
}