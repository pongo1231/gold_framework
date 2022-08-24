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

gold_model::gold_model(gold_unique_ptr<gold_mesh> &&mesh, gold_unique_ptr<gold_shader_program> &&shader_program)
    : mesh(std::move(mesh)), shader_program(std::move(shader_program))
{
	this->shader_program->bind();
	this->shader_program->set_uniform_vector3("uni_light_pos", { 0.f, 20.f, 0.f });
	this->shader_program->set_uniform_vector3("uni_light_col", { 1.f, 1.f, 1.f });
	this->shader_program->set_uniform_float("uni_ambient_modifier", .5f);
	this->shader_program->set_uniform_float("uni_spec_modifier", specular_multiplier);
	this->shader_program->set_uniform_float("uni_shininess", shininess);
	this->shader_program->unbind();
}

void gold_model::render(const gold_camera *camera)
{
	if (!camera)
		gold_assert("gold_model::render camera == null!");

	if (shader_program)
	{
		shader_program->bind();

		glUniformMatrix4fv(shader_program->get_perspective_uniform_location(), 1, GL_FALSE,
		                   glm::value_ptr(camera->get_perspective()));

		glUniformMatrix4fv(shader_program->get_view_uniform_location(), 1, GL_FALSE,
		                   glm::value_ptr(camera->get_view()));
	}

	auto matrix = get_model_matrix();
	glUniformMatrix4fv(shader_program->get_model_uniform_location(), 1, GL_FALSE, glm::value_ptr(matrix));

	if (texture)
		texture->bind(shader_program.handle());

	if (mesh)
		mesh->render();

	if (texture)
		texture->unbind();

	if (shader_program)
		shader_program->unbind();
}

const gold_shader_program *gold_model::get_shader_program() const
{
	return shader_program.handle();
}

void gold_model::set_texture(gold_ref_ptr<gold_texture> texture)
{
	this->texture = texture;
}

const gold_vector3 &gold_model::get_position() const
{
	return position;
}

void gold_model::set_position(const gold_vector3 &pos)
{
	position = pos;
}

const gold_vector3 &gold_model::get_rotation() const
{
	return rotation;
}

void gold_model::set_rotation(const gold_vector3 &rotation)
{
	this->rotation = rotation;
}

const gold_vector3 &gold_model::get_scale() const
{
	return m_scale;
}

void gold_model::set_scale(const gold_vector3 &_scale)
{
	m_scale = _scale;
}

glm::mat4 gold_model::get_model_matrix() const
{
	glm::mat4 matrix(1.f);
	matrix = glm::scale(matrix, { m_scale.x, m_scale.y, m_scale.z });
	matrix = glm::rotate(matrix, 6.28319f,
	                     { rotation.x == 0.f ? 1.f : std::fmod(rotation.x / 360.f, 1.f),
	                       rotation.y == 0.f ? 1.f : std::fmod(rotation.y / 360.f, 1.f),
	                       rotation.z == 0.f ? 1.f : std::fmod(rotation.z / 360.f, 1.f) });
	matrix = glm::translate(matrix, { position.x, position.y, position.z });

	return matrix;
}

void gold_model::set_specular_multiplier(float specular_multiplier)
{
	this->specular_multiplier = specular_multiplier;

	shader_program->bind();
	shader_program->set_uniform_float("uni_spec_modifier", specular_multiplier);
	shader_program->unbind();
}

void gold_model::set_shininess(float shininess)
{
	this->shininess = shininess;

	shader_program->bind();
	shader_program->set_uniform_float("uni_shininess", shininess);
	shader_program->unbind();
}

gold_unique_ptr<gold_model> gold_model::load_from_obj(std::string_view filename)
{
	if (!gold_file::does_file_exist(filename))
		return nullptr;

	auto mesh           = gold_mesh::load_from_obj(filename);

	auto vert_shader    = gold_shader::load_from_file("shaders/cube_vert.glsl", GL_VERTEX_SHADER);
	auto frag_shader    = gold_shader::load_from_file("shaders/cube_frag.glsl", GL_FRAGMENT_SHADER);
	auto shader_program = gold_unique_ptr<gold_shader_program>::create(std::move(vert_shader), std::move(frag_shader));

	shader_program->bind();
	shader_program->set_uniform_vector3("uni_light_pos", { 0.f, 10.f, 10.f });
	shader_program->set_uniform_vector3("uni_light_col", { 1.f, 1.f, 1.f });
	shader_program->set_uniform_float("uni_ambient_modifier", .9f);
	shader_program->set_uniform_float("uni_spec_modifier", 0.f);
	shader_program->set_uniform_float("uni_shininess", 10.f);
	shader_program->unbind();

	return gold_unique_ptr<gold_model>::create(std::move(mesh), std::move(shader_program));
}