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
	/*
	* Default constructor without any vertices to draw
	*/
	gold_mesh() = default;
	/*
	 * Constructor
	 * <param name="vertices">Vertices to draw in passed order, to use indices pass them as next argument</param>
	 */
	gold_mesh(const gold_vector<gold_vertex> &vertices);
	/*
	 * Constructor
	 * <param name="vertices">Vertices to use for drawing</param>
	 * <param name="indices">Indices to indicate order of drawing</param>
	 */
	gold_mesh(const gold_vector<gold_vertex> &vertices, const gold_vector<std::uint32_t> &indices);
	/*
	* Destructor
	*/
	~gold_mesh();

	/*
	* Draw mesh
	*/
	void render() const;
	/*
	* <param name="state">Whether the vertices should be drawn as triangle strips</param>
	*/
	void set_triangle_strip(bool state);

	/*
	* <returns>Vertices of mesh</returns>
	*/
	const gold_vector<gold_vertex> &get_vertices() const;
	/*
	 * <returns>Indices of mesh</returns>
	 */
	const gold_vector<std::uint32_t> &get_indices() const;

	/*
	 * <param name="filename">Path to .obj file</param>
	 * <returns>Holding pointer to created mesh</returns>
	 */
	static gold_unique_ptr<gold_mesh> load_from_obj(std::string_view filename);
};