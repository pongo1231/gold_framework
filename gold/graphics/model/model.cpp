#include "model.h"

#include "gold/camera.h"
#include "gold/graphics/mesh.h"
#include "gold/graphics/shader_program.h"
#include "gold/graphics/texture.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

gold_model::gold_model(const std::shared_ptr<gold_mesh> mesh, const std::shared_ptr<gold_shader_program> shader_program)
    : mesh(mesh), shader_program(shader_program)
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

_NODISCARD const std::shared_ptr<gold_shader_program> gold_model::get_shader_program() const
{
	return shader_program;
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