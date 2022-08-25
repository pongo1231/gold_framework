#pragma once

#include "gold/graphics/shader_program.h"

using GLuint = unsigned int;

/*
* Texture holder
*/
class gold_texture
{
  private:
	GLuint texture_id = 0;
	int height = 0, width = 0;
	gold_unique_ptr<std::uint8_t *> rgb;

  public:
	/*
	* Constructor
	* <param name="filename">File to texture file (supports bmp, png, jpg and other common formats)</param>
	*/
	gold_texture(std::string_view filename);

	/*
	* Bind texture to global GLSL context
	* <param name="program">Shader program pass texture id to (as u_texture uniform)</param>
	*/
	void bind(const gold_shader_program *program) const;
	/*
	* Unbind texture from global GLSL context
	*/
	void unbind() const;
};