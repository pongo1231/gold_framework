#include "skybox.h"

#include "gold/camera.h"
#include "gold/graphics/mesh.h"
#include "gold/graphics/shader.h"
#include "gold/graphics/shader_program.h"
#include "gold/graphics/texture.h"
#include "gold/util/vertex.h"

#include <GL/glew.h>

/*
 * Vertices used for the skybox (position, texture uv, color, normals)
 */
static const gold_vector<gold_vertex> sky_vertices = {
	{ -1.f, 1.f, -1.f, 0.f, 0.f, .5f, .5f, 1.f, 1.f },  // 0
	{ -1.f, -1.f, -1.f, 1.f, 0.f, .3f, .3f, .4f, 1.f }, // 1
	{ 1.f, -1.f, -1.f, 1.f, 1.f, .3f, .3f, .4f, 1.f },  // 2

	{ -1.f, 1.f, -1.f, 1.f, 1.f, .5f, .5f, 1.f, 1.f }, // 3
	{ -1.f, -1.f, 1.f, 1.f, 1.f, .3f, .3f, .4f, 1.f }, // 4
	{ 1.f, 1.f, -1.f, 1.f, 1.f, .5f, .5f, 1.f, 1.f },  // 5

	{ -1.f, 1.f, 1.f, 1.f, 1.f, .5f, .5f, 1.f, 1.f }, // 6
	{ 1.f, -1.f, 1.f, 1.f, 1.f, .3f, .3f, .4f, 1.f }, // 7
	{ 1.f, 1.f, 1.f, 1.f, 1.f, .5f, .5f, 1.f, 1.f },  // 8
};

/*
 * Indices used for skybox
 */
static const gold_vector<std::uint32_t> sky_indices = { 0, 1, 2, 2, 5, 0,

	                                                    4, 1, 3, 3, 6, 4,

	                                                    2, 7, 8, 8, 5, 2,

	                                                    4, 6, 8, 8, 7, 4,

	                                                    0, 5, 8, 8, 6, 3,

	                                                    1, 4, 2, 2, 4, 7 };

gold_skybox::gold_skybox(gold_unique_ptr<gold_mesh> &&mesh, gold_unique_ptr<gold_shader_program> &&shader_program)
    : gold_model(std::move(mesh), std::move(shader_program))
{
}

gold_unique_ptr<gold_skybox> gold_skybox::create()
{
	auto mesh = gold_unique_ptr<gold_mesh>::create(sky_vertices, sky_indices);
	auto shader_program =
	    gold_unique_ptr<gold_shader_program>::create("shaders/sky_vert.glsl", "shaders/sky_frag.glsl");

	auto alloc = reinterpret_cast<gold_skybox *>(gold_global_allocate(sizeof(gold_skybox)));
	new (alloc) gold_skybox(std::move(mesh), std::move(shader_program));
	return gold_unique_ptr<gold_skybox>(alloc);
}

void gold_skybox::render(const gold_camera *camera)
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);

	set_position(camera->get_eye());
	gold_model::render(camera);

	glDepthMask(GL_TRUE);
}