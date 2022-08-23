#include "texture.h"

#include "gold/util/file.h"
#include "gold/util/macros.h"

#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

gold_texture::gold_texture(std::string_view filename)
{
	if (!gold_file::does_file_exist(filename))
		gold_assert("gold_texture::gold_texture invalid file passed!");

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	int channels;
	auto data  = stbi_load(filename.data(), &width, &height, &channels, 0);
	auto alloc = reinterpret_cast<std::uint8_t **>(gold_global_allocate(width * height * channels));
	memcpy(alloc, data, width * height * channels);
	rgb = gold_unique_ptr<std::uint8_t *>(alloc);
	stbi_image_free(data);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, channels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE,
	             rgb.handle());
	glGenerateMipmap(GL_TEXTURE_2D);
}

void gold_texture::bind(const gold_shader_program *program) const
{
	if (!program)
		gold_assert("gold_texture::bind null shader program");

	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	program->set_uniform_longint("u_texture", 0);
}

void gold_texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}