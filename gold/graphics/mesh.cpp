#include "mesh.h"

#include "gold/memory.h"
#include "gold/util/file.h"
#include "gold/util/macros.h"
#include "gold/util/string.h"
#include "gold/util/vector3.h"
#include "gold/util/vertex.h"

#include <GL/glew.h>

gold_mesh::gold_mesh(const std::vector<gold_vertex> &vertices) : vertices(vertices)
{
	glGenBuffers(1, &vertex_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(gold_vertex), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

gold_mesh::gold_mesh(const std::vector<gold_vertex> &vertices, const std::vector<std::uint32_t> &indices)
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

std::unique_ptr<gold_mesh> gold_mesh::load_from_obj(std::string_view filename)
{
	if (!gold_file::does_file_exist(filename))
		return nullptr;

	auto file = gold_file::open_file(filename, "r");
	fseek(file.handle, 0, SEEK_END);
	auto file_size = ftell(file.handle);
	rewind(file.handle);

	std::string buffer;
	buffer.resize(file_size);
	fread(buffer.data(), sizeof(char), file_size, file.handle);

	std::vector<gold_vector3> vertex_positions;
	std::vector<gold_vector3> vertex_normals;
	std::vector<std::uint32_t> indices;

	for (auto &line : gold_string_split(buffer, '\n'))
	{
		gold_string_trim(line);
		if (line.empty())
			continue;

		const auto &values = gold_string_split(line, ' ');
		if (values[0] == "v")
			vertex_positions.emplace_back(std::stof(values[1]), std::stof(values[2]), std::stof(values[3]));
		else if (values[0] == "vn")
			vertex_normals.emplace_back(std::stof(values[1]), std::stof(values[2]), std::stof(values[3]));
		else if (values[0] == "f")
		{
			gold_vector3 this_indices;
			std::uint8_t count = 0;
			for (size_t i = 1; i < values.size(); i++)
			{
				const auto &index_values = gold_string_split(values[i], '/');
				this_indices[count++]    = std::stoi(index_values[0]);

				if (count == 3)
				{
					indices.push_back(this_indices.x);
					indices.push_back(this_indices.y);
					indices.push_back(this_indices.z);

					this_indices.x = this_indices.y;
					this_indices.y = this_indices.z;
					count          = 2;
				}
			}
		}
	}

	/*LOG("Vertices:");
	for (const auto &vertex : vertex_positions)
	    LOG(vertex.x << " " << vertex.y << " " << vertex.z);

	LOG("Normals:");
	for (const auto &normal : vertex_normals)
	    LOG(normal.x << " " << normal.y << " " << normal.z);*/

	/*LOG("Indices:");
	for (const auto &index : indices)
	    LOG(index.x << " " << index.y << " " << index.z);
	for (const auto &index : indices)
	    LOG(index);*/

	std::vector<gold_vertex> vertices;
	for (size_t i = 0; i < vertex_positions.size(); i++)
	{
		const auto &vertex_pos = vertex_positions[i];

		gold_vertex vertex;
		vertex.pos_x = vertex_pos.x;
		vertex.pos_y = vertex_pos.y;
		vertex.pos_z = vertex_pos.z;

		if (vertex_normals.size() > i)
		{
			const auto &vertex_normal = vertex_normals[i];
			vertex.norm_x             = vertex_normal.x;
			vertex.norm_y             = vertex_normal.y;
			vertex.norm_z             = vertex_normal.z;
		}

		vertices.push_back(vertex);
	}

	return std::make_unique<gold_mesh>(vertices, indices);
}