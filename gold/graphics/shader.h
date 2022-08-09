#pragma once

#include <memory>

#define _NODISCARD [[nodiscard]]

using GLuint = unsigned int;
using GLenum = unsigned int;

class gold_shader
{
  private:
	GLuint shader_id = 0;

  public:
	gold_shader(GLuint shader_id);
	~gold_shader();

	_NODISCARD GLuint get_id() const;

	_NODISCARD static std::shared_ptr<gold_shader> load_from_file(const char *file, GLenum shader_type);
};