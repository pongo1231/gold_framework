#include "mesh.h"

#include "gold/util/file.h"
#include "gold/util/macros.h"
#include "gold/util/string.h"
#include "gold/util/vector.h"
#include "gold/util/vector3.h"
#include "gold/util/vertex.h"

#include <GL/glew.h>

gold_mesh::gold_mesh(const std::vector<gold_vertex> &verts) : verts(verts)
{
	glGenBuffers(1, &vertex_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(gold_vertex), verts.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

gold_mesh::gold_mesh(const std::vector<gold_vertex> &verts, const std::vector<unsigned short> &ids)
    : verts(verts), ids(ids), ids_assigned(true)
{
	glGenBuffers(1, &vertex_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(gold_vertex), verts.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &index_buffer_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ids.size() * sizeof(unsigned short), ids.data(), GL_STATIC_DRAW);
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
		glDrawElements(is_triangle_strip ? GL_TRIANGLE_STRIP : GL_TRIANGLES, ids.size(), GL_UNSIGNED_SHORT, 0);
	else
		glDrawArrays(is_triangle_strip ? GL_TRIANGLE_STRIP : GL_TRIANGLES, 0, verts.size());

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

gold_mesh *gold_mesh::load_from_obj(std::string_view filename)
{
	if (!gold_file::does_file_exist(filename))
		return nullptr;

	auto file = gold_file::open_file(filename);
	fseek(file.handle, 0, SEEK_END);
	auto file_size = ftell(file.handle);
	rewind(file.handle);

	std::string buffer;
	buffer.reserve(file_size);
	fread(buffer.data(), sizeof(char), file_size, file.handle);

	gold_vector<gold_vector3> vertices;
	gold_vector<gold_vector3> indices;

	size_t index = 0;
	while ((index = buffer.find('\n', index)) != buffer.npos)
	{
		auto line = buffer.substr(0, index + 1);
		gold_string_trim(line);
		if (line.empty())
			continue;

		if (line[0] == 'v')
		{
			float x = 0.f, y = 0.f, z = 0.f;
			size_t vert_index = 0;

			auto vert         = line.substr(line.find_first_not_of());
		}
	}
}