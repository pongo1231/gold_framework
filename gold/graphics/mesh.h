#pragma once

#include "gold/memory.h"
#include "gold/util/vector3.h"

#include <string_view>

using GLuint = unsigned int;

struct gold_vertex;

class gold_mesh
{
	gold_vector<gold_vertex> vertices;
	gold_vector<std::uint32_t> indices;
	bool ids_assigned       = false;

	GLuint vertex_buffer_id = 0;
	GLuint index_buffer_id  = 0;

	bool is_triangle_strip  = false;

  public:
	gold_mesh() = default;
	gold_mesh(const gold_vector<gold_vertex> &vertices);
	gold_mesh(const gold_vector<gold_vertex> &vertices, const gold_vector<std::uint32_t> &indices);
	~gold_mesh();

	void render() const;
	void set_triangle_strip(bool state);

	static gold_unique_ptr<gold_mesh> load_from_obj(std::string_view filename);
};