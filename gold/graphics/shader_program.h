#pragma once

#include <memory>

#define _NODISCARD [[nodiscard]]

using GLuint = unsigned int;
using GLint  = int;

class gold_shader;
class gold_matrix4;
class gold_vector3;

class gold_shader_program
{
  private:
	GLuint program_id = 0;

	std::shared_ptr<gold_shader> vert_shader;
	std::shared_ptr<gold_shader> frag_shader;

	GLint uniform_matrix_perspective_location = 0;
	GLint uniform_matrix_view_location        = 0;
	GLint uniform_matrix_model_location       = 0;

	bool valid                                = false;

  public:
	gold_shader_program(std::shared_ptr<gold_shader> vert_shader, std::shared_ptr<gold_shader> frag_shader);

	void bind() const;
	void unbind() const;

	_NODISCARD GLint get_perspective_uniform_location() const;
	_NODISCARD GLint get_view_uniform_location() const;
	_NODISCARD GLint get_model_uniform_location() const;

	void set_uniform_vector3(const char *uniform, const gold_vector3 &vector) const;
	void set_uniform_matrix4(const char *uniform, const gold_matrix4 &matrix) const;
	void set_uniform_float(const char *uniform, float value) const;
};