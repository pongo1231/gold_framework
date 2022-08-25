#pragma once

#include "gold/memory.h"

#include <string_view>

using GLuint = unsigned int;
using GLenum = unsigned int;

/*
* Shader holder
*/
class gold_shader
{
  private:
	GLuint shader_id = 0;

  public:
	/*
	* Constructor
	* <param name="shader_id">Id of compiled GLSL shader</param>
	*/
	gold_shader(GLuint shader_id);
	/*
	* Destructor
	*/
	~gold_shader();

	/*
	* <returns>Id of compiled shader</returns>
	*/
	GLuint get_id() const;

	/*
	 * <param name="file">Path to source GLSL shader file</param>
	 * <param name="shader_type">Type of shader (either GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)</param>
	 * <returns>Holding pointer to compiled shader</returns>
	 */
	static gold_unique_ptr<gold_shader> load_from_file(std::string_view file, GLenum shader_type);
};