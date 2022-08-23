#pragma once

#include "gold/graphics/shader.h"
#include "gold/memory.h"

using GLuint = unsigned int;
using GLint  = int;

class gold_shader;
class gold_matrix4;
class gold_vector3;

class gold_shader_program
{
  private:
	GLuint program_id = 0;

	gold_ref_ptr<gold_shader> vert_shader;
	gold_ref_ptr<gold_shader> frag_shader;

	GLint uniform_matrix_perspective_location = 0;
	GLint uniform_matrix_view_location        = 0;
	GLint uniform_matrix_model_location       = 0;

	bool valid                                = false;

  public:
	gold_shader_program(gold_ref_ptr<gold_shader> vert_shader, gold_ref_ptr<gold_shader> frag_shader);

	void bind() const;
	void unbind() const;

	GLint get_perspective_uniform_location() const;
	GLint get_view_uniform_location() const;
	GLint get_model_uniform_location() const;

	void set_uniform_vector3(const char *uniform, const gold_vector3 &vector) const;
	void set_uniform_matrix4(const char *uniform, const gold_matrix4 &matrix) const;
	void set_uniform_float(const char *uniform, float value) const;
	void set_uniform_longint(const char *uniform, long value) const;
};