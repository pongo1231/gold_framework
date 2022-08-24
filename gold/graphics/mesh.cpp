#include "mesh.h"

#include "gold/memory.h"
#include "gold/util/file.h"
#include "gold/util/macros.h"
#include "gold/util/random.h"
#include "gold/util/string.h"
#include "gold/util/vector3.h"
#include "gold/util/vertex.h"

#include <GL/glew.h>

#include <sstream>

gold_mesh::gold_mesh(const gold_vector<gold_vertex> &vertices) : vertices(vertices)
{
	glGenBuffers(1, &vertex_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(gold_vertex), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

gold_mesh::gold_mesh(const gold_vector<gold_vertex> &vertices, const gold_vector<std::uint32_t> &indices)
    : vertices(vertices), indices(indices), ids_assigned(true)
{
	glGenBuffers(1, &vertex_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(gold_vertex), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &index_buffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(std::uint32_t), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

gold_mesh::~gold_mesh()
{
	glDeleteBuffers(1, &vertex_buffer_id);

	if (ids_assigned)
		glDeleteBuffers(1, &index_buffer_id);
}

void gold_mesh::render() const
{
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);

	if (ids_assigned)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(gold_vertex), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(gold_vertex), reinterpret_cast<void *>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(gold_vertex), reinterpret_cast<void *>(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(gold_vertex), reinterpret_cast<void *>(9 * sizeof(float)));
	glEnableVertexAttribArray(3);

	if (ids_assigned)
		glDrawElements(is_triangle_strip ? GL_TRIANGLE_STRIP : GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(is_triangle_strip ? GL_TRIANGLE_STRIP : GL_TRIANGLES, 0, vertices.size());

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (ids_assigned)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

void gold_mesh::set_triangle_strip(bool state)
{
	is_triangle_strip = state;
}

const gold_vector<gold_vertex> &gold_mesh::get_vertices() const
{
	return vertices;
}

const gold_vector<std::uint32_t> &gold_mesh::get_indices() const
{
	return indices;
}

gold_unique_ptr<gold_mesh> gold_mesh::load_from_obj(std::string_view filename)
{
	if (!gold_file::does_file_exist(filename))
		return nullptr;

	auto file = gold_file::open_file(filename, "r");
	fseek(file.handle, 0, SEEK_END);
	auto file_size = ftell(file.handle);
	rewind(file.handle);

	gold_string buffer;
	buffer.resize(file_size);
	fread(buffer.c_string(), sizeof(char), file_size, file.handle);

	gold_vector<gold_vector3> vertex_positions;
	gold_vector<std::uint32_t> vertex_indices;
	gold_vector<gold_vector3> vertex_normals;
	gold_vector<std::uint32_t> normal_indices;
	gold_vector<gold_vector3> vertex_uvs;
	gold_vector<std::uint32_t> uv_indices;

	for (auto &line : buffer.split("\n"))
	{
		line = line.trim();
		if (line.is_empty())
			continue;

		const auto &values = line.split(" ");
		if (values[0] == "v")
			vertex_positions.emplace(std::stof(values[1].c_string()), std::stof(values[2].c_string()),
			                         std::stof(values[3].c_string()));
		else if (values[0] == "vn")
			vertex_normals.emplace(std::stof(values[1].c_string()), std::stof(values[2].c_string()),
			                       std::stof(values[3].c_string()));
		else if (values[0] == "vt")
			vertex_uvs.emplace(std::stof(values[1].c_string()), std::stof(values[2].c_string()), 0.f);
		else if (values[0] == "f")
		{
			auto parse_indices = [](const auto &values, auto &list, uint8_t index)
			{
				gold_vector3 indices;
				for (std::uint8_t i = 1; i < values.size(); i++)
				{
					const auto &index_values    = values[i].split("/");

					indices[std::min(i - 1, 2)] = std::stoi(index_values[index].c_string()) - 1;

					if (i > 2)
					{
						list.push(indices.x);
						list.push(indices.y);
						list.push(indices.z);

						indices.x = std::stoi(values[std::max(1, i - 3)].c_string()) - 1;
						indices.y = indices.z;
					}
				}
			};

			// vertices
			parse_indices(values, vertex_indices, 0);

			// check with first index, should be the same format throughout
			// normals (vertex//normal)
			if (values[1].contains("//"))
				parse_indices(values, normal_indices, 1);
			else
			{
				auto indices_count = values[1].count("/");

				// texture uv (vertex/uv)
				if (indices_count > 0)
					parse_indices(values, uv_indices, 1);
				// normals (vertex/uv/normal)
				if (indices_count > 1)
					parse_indices(values, normal_indices, 2);
			}
		}
	}

	gold_vector<gold_vertex> vertices;
	for (size_t i = 0; i < vertex_positions.size(); i++)
	{
		const auto &vertex_pos = vertex_positions[i];

		gold_vertex vertex;
		vertex.pos_x = vertex_pos.x;
		vertex.pos_y = vertex_pos.y;
		vertex.pos_z = vertex_pos.z;

		if (!normal_indices.empty())
		{
			auto normal_index = normal_indices[i];
			vertex.norm_x     = vertex_normals[normal_index].x;
			vertex.norm_y     = vertex_normals[normal_index].y;
			vertex.norm_z     = vertex_normals[normal_index].z;
		}

		if (!uv_indices.empty())
		{
			auto uv_index = uv_indices[i];
			vertex.tex_u  = vertex_uvs[uv_index].x;
			vertex.tex_v  = vertex_uvs[uv_index].y;
		}

		vertex.col_r = 1.f;
		vertex.col_g = 1.f;
		vertex.col_b = 1.f;
		vertex.col_a = 1.f;

		vertices.push(vertex);
	}

	return gold_unique_ptr<gold_mesh>::create(vertices, vertex_indices);
}