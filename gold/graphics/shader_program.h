#pragma once

#include "gold/graphics/shader.h"
#include "gold/memory.h"

using GLuint = unsigned int;
using GLint  = int;

class gold_shader;
class gold_matrix4;
class gold_vector3;

/*
 * Shader program holding vertex & fragment shaders
 */
class gold_shader_program
{
  private:
	GLuint program_id = 0;

	gold_unique_ptr<gold_shader> vertex_shader;
	gold_unique_ptr<gold_shader> fragment_shader;

	GLint uniform_matrix_perspective_location = 0;
	GLint uniform_matrix_view_location        = 0;
	GLint uniform_matrix_model_location       = 0;

	bool valid                                = false;

  public:
	/*
	 * Constructor
	 * <param name="vertex_shader">Holding pointer to vertex shader (pass using std::move)</param>
	 * <param name="fragment_shader">Holding pointer to fragment shader (pass using std::move)</param>
	 */
	gold_shader_program(gold_unique_ptr<gold_shader> &&vertex_shader, gold_unique_ptr<gold_shader> &&fragment_shader);
	/*
	 * Constructor
	 * <param name="vertex_shader">Path to source GLSL vertex shader file</param>
	 * <param name="fragment_shader">Path to source GLSL fragment shader file</param>
	 */
	gold_shader_program(std::string_view vertex_shader, std::string_view fragment_shader);
	/*
	* Bind shaders to global GL context
	*/
	void bind() const;
	/*
	* Unbind shader from global GL context
	*/
	void unbind() const;

	/*
	 * <returns>Location of perspective uniform in vertex shader</returns>
	 */
	GLint get_perspective_uniform_location() const;
	/*
	 * <returns>Location of view uniform in vertex shader</returns>
	 */
	GLint get_view_uniform_location() const;
	/*
	 * <returns>Location of model uniform in vertex shader</returns>
	 */
	GLint get_model_uniform_location() const;

	/*
	 * Set uniform value to vector3
	 * <param name="uniform">Name of uniform</param>
	 * <param name="vector">Value</param>
	 */
	void set_uniform_vector3(const char *uniform, const gold_vector3 &vector) const;
	/*
	 * Set uniform value to matrix4
	 * <param name="uniform">Name of uniform</param>
	 * <param name="matrix">Value</param>
	 */
	void set_uniform_matrix4(const char *uniform, const gold_matrix4 &matrix) const;
	/*
	 * Set uniform value to float
	 * <param name="uniform">Name of uniform</param>
	 * <param name="value">Value</param>
	 */
	void set_uniform_float(const char *uniform, float value) const;
	/*
	 * Set uniform value to longint
	 * <param name="uniform">Name of uniform</param>
	 * <param name="value">Value</param>
	 */
	void set_uniform_longint(const char *uniform, long value) const;
};