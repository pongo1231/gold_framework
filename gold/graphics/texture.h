#pragma once

#include "gold/graphics/shader_program.h"

using GLuint = unsigned int;

class gold_texture
{
  private:
	GLuint texture_id = 0;
	int height = 0, width = 0;
	gold_vector<float> rgb;

  public:
	gold_texture(const char *filename);

	void bind(const gold_shader_program &program) const;
	void unbind() const;
};