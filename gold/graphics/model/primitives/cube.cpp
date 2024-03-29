#include "cube.h"

#include "gold/camera.h"
#include "gold/graphics/mesh.h"
#include "gold/graphics/shader.h"
#include "gold/graphics/shader_program.h"
#include "gold/graphics/texture.h"
#include "gold/util/macros.h"
#include "gold/util/vertex.h"

#include <GL/glew.h>

/*
* Vertices used for the cube (position, texture uv, color, normals)
*/
static const gold_vector<gold_vertex> cube_vertices = {
	{ -1.f, 1.f, -1.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 1.f, 0.f },
	{ 1.f, 1.f, -1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 1.f, 0.f },
	{ -1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 1.f, 0.f },
	{ 1.f, 1.f, 1.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 1.f, 0.f },

	{ -1.f, -1.f, -1.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, -1.f, 0.f },
	{ 1.f, -1.f, -1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, -1.f, 0.f },
	{ -1.f, -1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, -1.f, 0.f },
	{ 1.f, -1.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, -1.f, 0.f },

	{ 1.f, 1.f, 1.f, 0.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f },
	{ 1.f, 1.f, -1.f, 1.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f },
	{ 1.f, -1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f },
	{ 1.f, -1.f, -1.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f },

	{ -1.f, 1.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f, -1.f, 0.f, 0.f },
	{ -1.f, 1.f, -1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, -1.f, 0.f, 0.f },
	{ -1.f, -1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, -1.f, 0.f, 0.f },
	{ -1.f, -1.f, -1.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f, -1.f, 0.f, 0.f },

	{ -1.f, 1.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f },
	{ 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f },
	{ -1.f, -1.f, 1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f },
	{ 1.f, -1.f, 1.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f },

	{ -1.f, 1.f, -1.f, 1.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, -1.f },
	{ 1.f, 1.f, -1.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, -1.f },
	{ -1.f, -1.f, -1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, -1.f },
	{ 1.f, -1.f, -1.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f, 0.f, 0.f, -1.f },
};

/*
* Indices used for cube
*/
static const gold_vector<std::uint32_t> cube_indices = { 8,  9,  10, 9,  11, 10,

	                                                     14, 13, 12, 14, 15, 13,

	                                                     1,  2,  0,  3,  2,  1,

	                                                     4,  6,  5,  5,  6,  7,

	                                                     17, 18, 16, 19, 18, 17,

	                                                     20, 22, 21, 21, 22, 23 };

gold_cube::gold_cube(gold_unique_ptr<gold_mesh> &&mesh, gold_unique_ptr<gold_shader_program> &&shader_program)
    : gold_model(std::move(mesh), std::move(shader_program))
{
}


gold_unique_ptr<gold_cube> gold_cube::create()
{
	auto cube_mesh = gold_unique_ptr<gold_mesh>::create(cube_vertices, cube_indices);
	cube_mesh->set_triangle_strip(true);
	auto cube_vert_shader = gold_shader::load_from_file("shaders/cube_vert.glsl", GL_VERTEX_SHADER);
	gold_unique_ptr<gold_shader> cube_frag_shader = gold_shader::load_from_file("shaders/cube_frag.glsl", GL_FRAGMENT_SHADER);
	auto cube_shader_program =
	    gold_unique_ptr<gold_shader_program>::create(std::move(cube_vert_shader), std::move(cube_frag_shader));

	auto alloc = reinterpret_cast<gold_cube *>(gold_global_allocate(sizeof(gold_cube)));
	new (alloc) gold_cube(std::move(cube_mesh), std::move(cube_shader_program));
	return gold_unique_ptr<gold_cube>(alloc);
}

void gold_cube::render(const gold_camera *camera)
{
	glEnable(GL_DEPTH_TEST);

	get_shader_program()->bind();
	get_shader_program()->set_uniform_vector3("uni_cam_pos", camera->get_eye());

	gold_model::render(camera);
}