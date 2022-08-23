#include "shader_program.h"

#include "gold/graphics/shader.h"
#include "gold/util/macros.h"
#include "gold/util/matrix.h"
#include "gold/util/vector3.h"

#include <GL/glew.h>

gold_shader_program::gold_shader_program(gold_unique_ptr<gold_shader> &&vertex_shader,
                                         gold_unique_ptr<gold_shader> &&fragment_shader)
    : vertex_shader(std::move(vertex_shader)), fragment_shader(std::move(fragment_shader))
{
	program_id = glCreateProgram();
	glAttachShader(program_id, this->vertex_shader->get_id());
	glAttachShader(program_id, this->fragment_shader->get_id());
	glLinkProgram(program_id);
	glValidateProgram(program_id);

	GLint succeeded;
	glGetProgramiv(program_id, GL_VALIDATE_STATUS, &succeeded);

	if (!succeeded)
	{
		LOG_ERROR("Shader compilation failed");
		return;
	}

	uniform_matrix_perspective_location = glGetUniformLocation(program_id, "uniform_matrix_perspective");
	uniform_matrix_view_location        = glGetUniformLocation(program_id, "uniform_matrix_view");
	uniform_matrix_model_location       = glGetUniformLocation(program_id, "uniform_matrix_model");

	valid                               = true;
}

gold_shader_program::gold_shader_program(std::string_view vertex_shader, std::string_view fragment_shader)
    : vertex_shader(gold_shader::load_from_file(vertex_shader, GL_VERTEX_SHADER)),
      fragment_shader(gold_shader::load_from_file(fragment_shader, GL_FRAGMENT_SHADER))
{
	program_id = glCreateProgram();
	glAttachShader(program_id, this->vertex_shader->get_id());
	glAttachShader(program_id, this->fragment_shader->get_id());
	glLinkProgram(program_id);
	glValidateProgram(program_id);

	GLint succeeded;
	glGetProgramiv(program_id, GL_VALIDATE_STATUS, &succeeded);

	if (!succeeded)
	{
		LOG_ERROR("Shader compilation failed");
		return;
	}

	uniform_matrix_perspective_location = glGetUniformLocation(program_id, "uniform_matrix_perspective");
	uniform_matrix_view_location        = glGetUniformLocation(program_id, "uniform_matrix_view");
	uniform_matrix_model_location       = glGetUniformLocation(program_id, "uniform_matrix_model");

	valid                               = true;
}

void gold_shader_program::bind() const
{
	if (!valid)
		return;

	glUseProgram(program_id);
}

void gold_shader_program::unbind() const
{
	if (!valid)
		return;

	glUseProgram(0);
}

GLint gold_shader_program::get_perspective_uniform_location() const
{
	return uniform_matrix_perspective_location;
}

GLint gold_shader_program::get_view_uniform_location() const
{
	return uniform_matrix_view_location;
}

GLint gold_shader_program::get_model_uniform_location() const
{
	return uniform_matrix_model_location;
}

void gold_shader_program::set_uniform_vector3(const char *uniform, const gold_vector3 &vector) const
{
	auto uniform_location = glGetUniformLocation(program_id, uniform);
	if (!uniform_location)
		return;

	glUniform3f(uniform_location, vector.x, vector.y, vector.z);
}

void gold_shader_program::set_uniform_matrix4(const char *uniform, const gold_matrix4 &matrix) const
{
	auto uniform_location = glGetUniformLocation(program_id, uniform);
	if (!uniform_location)
		return;

	glUniformMatrix4fv(uniform_location, 1, GL_FALSE, matrix.matrix);
}

void gold_shader_program::set_uniform_float(const char *uniform, float value) const
{
	auto uniform_location = glGetUniformLocation(program_id, uniform);
	if (!uniform_location)
		return;

	glUniform1f(uniform_location, value);
}

void gold_shader_program::set_uniform_longint(const char *uniform, long value) const
{
	auto uniform_location = glGetUniformLocation(program_id, uniform);
	if (!uniform_location)
		return;

	glUniform1i(uniform_location, value);
}