#include "model.h"

#include "gold/camera.h"
#include "gold/graphics/mesh.h"
#include "gold/graphics/shader.h"
#include "gold/graphics/shader_program.h"
#include "gold/graphics/texture.h"
#include "gold/util/file.h"
#include "gold/util/vertex.h"
#include "gold/util/light.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

static gold_ref_ptr<gold_texture> default_texture;

gold_model::gold_model(gold_unique_ptr<gold_mesh> &&mesh, gold_unique_ptr<gold_shader_program> &&shader_program)
    : mesh(std::move(mesh)), shader_program(std::move(shader_program))
{
	this->shader_program->bind();
	this->shader_program->set_uniform_float("uni_ambient_modifier", ambient);
	this->shader_program->set_uniform_float("uni_spec_modifier", specular_multiplier);
	this->shader_program->set_uniform_float("uni_shininess", shininess);
	this->shader_program->unbind();

	if (!default_texture)
		default_texture = gold_ref_ptr<gold_texture>::create("textures/default.png");

	texture = default_texture;

	if (!this->mesh->get_vertices().empty())
	{
		gold_vector3 min { std::numeric_limits<float>::max(), std::numeric_limits<float>::max(),
			               std::numeric_limits<float>::max() };
		gold_vector3 max { std::numeric_limits<float>::min(), std::numeric_limits<float>::min(),
			               std::numeric_limits<float>::min() };
		for (const auto &vertex : this->mesh->get_vertices())
		{
			min.x = std::min(vertex.pos_x, min.x);
			max.x = std::max(max.x, vertex.pos_x);

			min.y = std::min(vertex.pos_y, min.y);
			max.y = std::max(max.y, vertex.pos_y);

			min.z = std::min(vertex.pos_z, min.z);
			max.z = std::max(max.z, vertex.pos_z);
		}

		box_min = min;
		box_max = max;
	}
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

		this->shader_program->set_uniform_vector3("uni_light_pos", gold_global_light_position);
		this->shader_program->set_uniform_vector3("uni_light_col", gold_global_light_color);
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
	return scale;
}

void gold_model::set_scale(const gold_vector3 &scale)
{
	this->scale = scale;
}

glm::mat4 gold_model::get_model_matrix() const
{
	glm::mat4 matrix(1.f);
	matrix = glm::scale(matrix, { scale.x, scale.y, scale.z });
	matrix = glm::rotate(matrix, 6.28319f,
	                     { rotation.x == 0.f ? 1.f : std::fmod(rotation.x / 360.f, 1.f),
	                       rotation.y == 0.f ? 1.f : std::fmod(rotation.y / 360.f, 1.f),
	                       rotation.z == 0.f ? 1.f : std::fmod(rotation.z / 360.f, 1.f) });
	matrix = glm::translate(matrix, { position.x / scale.x, position.y / scale.y, position.z / scale.z });

	return matrix;
}

void gold_model::set_ambient_multiplier(float ambient)
{
	this->ambient = ambient;

	shader_program->bind();
	shader_program->set_uniform_float("uni_ambient_modifier", ambient);
	shader_program->unbind();
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

void gold_model::get_bounding_box(gold_vector3 &min, gold_vector3 &max) const
{
	min = box_min;
	max = box_max;
}

bool gold_model::is_colliding_with(const gold_model &model, const gold_vector3 &offset) const
{
	auto const &other_pos     = model.position;
	auto const &other_box_min = model.box_min;
	auto const &other_box_max = model.box_max;
	auto const &other_scale   = model.scale;

	return position.x + offset.x + box_max.x * scale.x > other_pos.x + other_box_min.x * other_scale.x
	    && position.x + offset.x + box_min.x * scale.x < other_pos.x + other_box_max.x * other_scale.x
	    && position.y + offset.y + box_max.y * scale.y > other_pos.y + other_box_min.y * other_scale.y
	    && position.y + offset.y + box_min.y * scale.y < other_pos.y + other_box_max.y * other_scale.y
	    && position.z + offset.z + box_max.z * scale.z > other_pos.z + other_box_min.z * other_scale.z
	    && position.z + offset.z + box_min.z * scale.z < other_pos.z + other_box_max.z * other_scale.z;
}

gold_unique_ptr<gold_model> gold_model::load_from_obj(std::string_view filename)
{
	return load_from_obj(filename, "shaders/cube_vert.glsl", "shaders/cube_frag.glsl");
}

gold_unique_ptr<gold_model> gold_model::load_from_obj(std::string_view filename, std::string_view vertex_shader, std::string_view fragment_shader)
{
	if (!gold_file::does_file_exist(filename))
		return nullptr;

	auto mesh           = gold_mesh::load_from_obj(filename);

	auto vert_shader    = gold_shader::load_from_file(vertex_shader, GL_VERTEX_SHADER);
	auto frag_shader    = gold_shader::load_from_file(fragment_shader, GL_FRAGMENT_SHADER);
	auto shader_program = gold_unique_ptr<gold_shader_program>::create(std::move(vert_shader), std::move(frag_shader));

	return gold_unique_ptr<gold_model>::create(std::move(mesh), std::move(shader_program));
}