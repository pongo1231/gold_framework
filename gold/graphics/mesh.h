#pragma once

#include <string_view>
#include <vector>

using GLuint = unsigned int;

struct gold_vertex;

class gold_mesh
{
  private:
	std::vector<gold_vertex> verts;
	std::vector<unsigned short> ids;
	bool ids_assigned       = false;

	GLuint vertex_buffer_id = 0;
	GLuint index_buffer_id  = 0;

	bool is_triangle_strip  = false;

  public:
	gold_mesh(const std::vector<gold_vertex> &verts);
	gold_mesh(const std::vector<gold_vertex> &verts, const std::vector<unsigned short> &ids);
	~gold_mesh();

	void render() const;
	void set_triangle_strip(bool state);

	static gold_mesh *load_from_obj(std::string_view filename);
};