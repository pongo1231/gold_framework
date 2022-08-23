#pragma once

#include "gold/graphics/shader_program.h"

using GLuint = unsigned int;

class gold_texture
{
  private:
	GLuint texture_id = 0;
	int height = 0, width = 0;
	gold_unique_ptr<std::uint8_t *> rgb;

  public:
	gold_texture(std::string_view filename);

	void bind(const gold_shader_program *program) const;
	void unbind() const;
};