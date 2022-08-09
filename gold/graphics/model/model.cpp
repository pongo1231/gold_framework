#include "model.h"

#include "gold/camera.h"
#include "gold/graphics/mesh.h"
#include "gold/graphics/shader.h"
#include "gold/graphics/shader_program.h"
#include "gold/graphics/texture.h"
#include "gold/util/file.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

gold_model::gold_model(std::unique_ptr<gold_mesh> &mesh, std::unique_ptr<gold_shader_program> &shader_program)
    : mesh(std::move(mesh)), shader_program(std::move(shader_program))
{
}

void gold_model::render(const gold_camera *camera) const
{
	shader_program->bind();

	glUniformMatrix4fv(shader_program->get_perspective_uniform_location(), 1, GL_FALSE,
	                   glm::value_ptr(camera->get_perspective()));

	glUniformMatrix4fv(shader_program->get_view_uniform_location(), 1, GL_FALSE, glm::value_ptr(camera->get_view()));

	auto matrix = glm::mat4(1.f);
	matrix      = glm::scale(matrix, { m_scale.x, m_scale.y, m_scale.z });
	matrix      = glm::rotate(matrix, angle, { rotation.x, rotation.y, rotation.z });
	matrix      = glm::translate(matrix, { position.x, position.y, position.z });
	glUniformMatrix4fv(shader_program->get_model_uniform_location(), 1, GL_FALSE, glm::value_ptr(matrix));

	if (texture)
		texture->bind();

	mesh->render();

	if (texture)
		texture->unbind();

	shader_program->unbind();
}

const gold_shader_program *gold_model::get_shader_program() const
{
	return shader_program.get();
}

void gold_model::set_texture(const std::shared_ptr<gold_texture> texture)
{
	this->texture = texture;
}

const gold_vector3 &gold_model::get_pos() const
{
	return position;
}

void gold_model::set_pos(const gold_vector3 &pos)
{
	position = pos;
}

const gold_vector3 &gold_model::get_rotation() const
{
	return rotation;
}

float gold_model::get_angle() const
{
	return angle;
}

void gold_model::set_rotation(float angle, const gold_vector3 &rot, bool in_radians)
{
	angle    = in_radians ? angle : glm::radians(angle);
	rotation = rot;
}

const gold_vector3 &gold_model::get_scale() const
{
	return m_scale;
}

void gold_model::set_scale(const gold_vector3 &_scale)
{
	m_scale = _scale;
}

std::unique_ptr<gold_model> gold_model::load_from_obj(std::string_view filename)
{
	if (!gold_file::does_file_exist(filename))
		return nullptr;

	auto mesh                = gold_mesh::load_from_obj(filename);

	auto cube_vert_shader    = gold_shader::load_from_file("shaders/cube_vert.glsl", GL_VERTEX_SHADER);
	auto cube_frag_shader    = gold_shader::load_from_file("shaders/cube_frag.glsl", GL_FRAGMENT_SHADER);
	auto cube_shader_program = std::make_unique<gold_shader_program>(cube_vert_shader, cube_frag_shader);

	cube_shader_program->bind();
	cube_shader_program->set_uniform_vector3("uni_light_pos", { 0.f, 0.f, 0.f });
	cube_shader_program->set_uniform_vector3("uni_light_col", { .0f, .5f, .5f });
	cube_shader_program->set_uniform_float("uni_ambient_modifier", .9f);
	cube_shader_program->set_uniform_float("uni_spec_modifier", 1.f);
	cube_shader_program->set_uniform_float("uni_shininess", 10.f);
	cube_shader_program->unbind();

	return std::make_unique<gold_model>(mesh, cube_shader_program);
}