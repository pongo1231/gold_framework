#include "texture.h"

#include "gold/util/macros.h"

#include <GL/glew.h>
//#include <png.h>

#include <vector>

gold_texture::gold_texture(const char *filename)
{
	/*glGenTextures(1, &texture_id);

	auto png      = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	auto png_info = png_create_info_struct(png);

	// png_set_palette_to_rgb(png);

	auto file     = fopen(filename, "r");
	// png_init_io(png, file);

	return;
	png_read_png(png, png_info, PNG_TRANSFORM_IDENTITY, NULL);
	auto rgb_rows = png_get_rows(png, png_info);

	height        = png_get_image_height(png, png_info);
	width         = png_get_image_width(png, png_info);
	rgb.reserve(height * width * 3);
	for (int r = 0; r < height; r++)
	{
	    for (int c = 0; c < width * 3; c++)
	    {
	        rgb.push_back(rgb_rows[r][c]);
	    }
	}

	fclose(file);
	png_destroy_read_struct(&png, &png_info, NULL);*/
}

void gold_texture::bind() const
{
	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, rgb.data());*/
}

void gold_texture::unbind() const
{
	// glBindTexture(GL_TEXTURE_2D, 0);
}