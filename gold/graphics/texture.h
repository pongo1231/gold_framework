#pragma once

#include <vector>

using GLuint = unsigned int;

class gold_texture
{
  private:
	GLuint texture_id = 0;
	int height = 0, width = 0;
	std::vector<float> rgb;

  public:
	gold_texture(const char *filename);

	void bind() const;
	void unbind() const;
};