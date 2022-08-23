#include "shader.h"

#include "gold/util/macros.h"

#include <GL/glew.h>

#include <fstream>
#include <sstream>
#include <string>

gold_shader::gold_shader(GLuint shader_id) : shader_id(shader_id)
{
}

gold_shader::~gold_shader()
{
	glDeleteShader(shader_id);
}

GLuint gold_shader::get_id() const
{
	return shader_id;
}

gold_ref_ptr<gold_shader> gold_shader::load_from_file(std::string_view file, GLenum shader_type)
{
	std::ifstream shader_file(file.data());
	if (shader_file.bad())
		return nullptr;

	std::stringstream shader_src_buffer;
	shader_src_buffer << shader_file.rdbuf();

	auto shader_src    = shader_src_buffer.str();
	auto shader_id_str = shader_src.c_str();

	auto shader_id     = glCreateShader(shader_type);
	glShaderSource(shader_id, 1, &shader_id_str, 0);
	glCompileShader(shader_id);

	return gold_ref_ptr<gold_shader>::create(shader_id);
}