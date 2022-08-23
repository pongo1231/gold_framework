#include "texture.h"

#include "gold/util/macros.h"

#include <GL/glew.h>
#include <jpeg/jpeglib.h>
//#include <png.h>

static gold_vector<unsigned char> read_bmp(const char *filename, int &width, int &height)
{
	int i;
	FILE *f = fopen(filename, "rb");
	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

	// extract image height and width from header
	memcpy(&width, info + 18, sizeof(std::uint32_t));
	memcpy(&height, info + 22, sizeof(std::uint32_t));

	int heightSign = 1;
	if (height < 0)
	{
		heightSign = -1;
	}

	int size = 3 * width * abs(height);
	gold_vector<unsigned char> data; // allocate 3 bytes per pixel
	data.resize(size);
	fread(data.data(), sizeof(unsigned char), size, f); // read the rest of the data at once
	fclose(f);

	if (heightSign == 1)
	{
		for (i = 0; i < size; i += 3)
		{
			// code to flip the image data here....
		}
	}
	return data;
}

gold_texture::gold_texture(const char *filename)
{
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	auto data = read_bmp(filename, width, height);
	for (const auto pixel : data)
		rgb.push(pixel);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb.data());

	/*struct jpeg_decompress_struct cinfo;

	auto file = fopen(filename, "rb");
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, file);
	jpeg_read_header(&cinfo, TRUE);
	LOG("huh");
	jpeg_start_decompress(&cinfo);

	LOG("maymay");

	int width  = static_cast<int>(cinfo.output_width);

	int height = static_cast<int>(cinfo.output_height);

	LOG("Width: " << width << "Height: " << height << std::endl);*/

	/*auto png      = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	auto png_info = png_create_info_struct(png);

	png_set_palette_to_rgb(png);

	// auto file = fopen(filename, "r");
	png_init_io(png, file);

	png_read_png(png, png_info, PNG_TRANSFORM_IDENTITY, NULL);
	auto rgb_rows = png_get_rows(png, png_info);

	height        = png_get_image_height(png, png_info);
	width         = png_get_image_width(png, png_info);
	rgb.resize(height * width * 3);
	for (int r = 0; r < height; r++)
	{
	    for (int c = 0; c < width * 3; c++)
	    {
	        rgb.push_back(rgb_rows[r][c]);
	    }
	}*/

	// fclose(file);
	//  png_destroy_read_struct(&png, &png_info, NULL);
}

void gold_texture::bind(const gold_shader_program &program) const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb.data());
	program.set_uniform_longint("u_texture", 0);
	// glBindVertexArray(VAO);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, rgb.data());
}

void gold_texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}