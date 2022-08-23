#pragma once

#include "gold/memory.h"

#include <string_view>

using GLuint = unsigned int;
using GLenum = unsigned int;

class gold_shader
{
  private:
	GLuint shader_id = 0;

  public:
	gold_shader(GLuint shader_id);
	~gold_shader();

	GLuint get_id() const;

	static gold_unique_ptr<gold_shader> load_from_file(std::string_view file, GLenum shader_type);
};