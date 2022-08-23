#include "mesh.h"

#include "gold/memory.h"
#include "gold/util/file.h"
#include "gold/util/macros.h"
#include "gold/util/random.h"
#include "gold/util/string.h"
#include "gold/util/vector3.h"
#include "gold/util/vertex.h"

#include <GL/glew.h>

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

	for (auto &line : buffer.split('\n'))
	{
		line = line.trim();
		if (line.is_empty())
			continue;

		const auto &values = line.split(' ');
		if (values[0] == "v")
			vertex_positions.emplace(std::stof(values[1].c_string()), std::stof(values[2].c_string()),
			                         std::stof(values[3].c_string()));
		else if (values[0] == "vn")
			vertex_normals.emplace(std::stof(values[1].c_string()), std::stof(values[2].c_string()),
			                       std::stof(values[3].c_string()));
		else if (values[0] == "f")
		{
			gold_vector3 this_vertex_indices;
			gold_vector3 this_normal_indices;
			for (std::uint8_t i = 1; i < values.size(); i++)
			{
				const auto &index_values                = values[i].split('/');

				this_vertex_indices[std::min(i - 1, 2)] = std::stoi(index_values[0].c_string()) - 1;

				if (i > 2)
				{
					vertex_indices.push(this_vertex_indices.x);
					vertex_indices.push(this_vertex_indices.y);
					vertex_indices.push(this_vertex_indices.z);

					this_vertex_indices.x = std::stoi(values[std::max(1, i - 3)].c_string()) - 1;
					this_vertex_indices.y = this_vertex_indices.z;
				}

				this_normal_indices[std::min(i - 1, 2)] = std::stoi(index_values[2].c_string()) - 1;

				if (i > 2)
				{
					normal_indices.push(this_normal_indices.x);
					normal_indices.push(this_normal_indices.y);
					normal_indices.push(this_normal_indices.z);

					this_normal_indices.x = std::stoi(values[std::max(1, i - 3)].c_string()) - 1;
					this_normal_indices.y = this_normal_indices.z;
				}
			}
		}
	}

	/*LOG("Vertices:");
	for (const auto &vertex : vertex_positions)
	    LOG(vertex.x << " " << vertex.y << " " << vertex.z);*/

	/*LOG("Normals:");
	for (const auto &normal : vertex_normals)
	    LOG(normal.x << " " << normal.y << " " << normal.z);*/

	/*LOG("Indices:");
	for (const auto &index : indices)
	    LOG(index);*/

	gold_vector<gold_vertex> vertices;
	for (size_t i = 0; i < vertex_positions.size(); i++)
	{
		const auto &vertex_pos = vertex_positions[i];

		gold_vertex vertex;
		vertex.pos_x = vertex_pos.x;
		vertex.pos_y = vertex_pos.y;
		vertex.pos_z = vertex_pos.z;

		if (normal_indices.size() > i)
		{
			auto normal_index = normal_indices[i];
			vertex.norm_x     = vertex_normals[normal_index].x;
			vertex.norm_y     = vertex_normals[normal_index].y;
			vertex.norm_z     = vertex_normals[normal_index].z;
		}

		vertex.col_a = 0.f;
		vertex.col_r = gold_random.random_float(0.f, 1.f);
		vertex.col_b = gold_random.random_float(0.f, 1.f);
		vertex.col_g = 0.f;

		vertices.push(vertex);
	}

	return gold_unique_ptr<gold_mesh>::create(vertices, vertex_indices);
}