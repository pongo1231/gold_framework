#pragma once

#include "gold/memory.h"
#include "gold/util/vector3.h"

#include <memory>
#include <string_view>
#include <vector>

using GLuint = unsigned int;

struct gold_vertex;

class gold_mesh
{
	std::vector<gold_vertex> vertices;
	std::vector<std::uint32_t> indices;
	bool ids_assigned       = false;

	GLuint vertex_buffer_id = 0;
	GLuint index_buffer_id  = 0;

	bool is_triangle_strip  = false;

  public:
	gold_mesh(const std::vector<gold_vertex> &vertices);
	gold_mesh(const std::vector<gold_vertex> &vertices, const std::vector<std::uint32_t> &indices);
	~gold_mesh();

	void render() const;
	void set_triangle_strip(bool state);

	static std::unique_ptr<gold_mesh> load_from_obj(std::string_view filename);
};