#pragma once

#include "gold/graphics/model/model.h"
#include "gold/memory.h"

/*
* Cube Primitive
*/
class gold_cube : public gold_model
{
	/*
	* Constructs a cube primitive
	* <param name="mesh">Holding pointer for mesh (use std::move)</param>
	* <param name="shader_program">Holding pointer for shader program (use std::move)</param>
	*/
	gold_cube(gold_unique_ptr<gold_mesh> &&mesh, gold_unique_ptr<gold_shader_program> &&shader_program);

  public:
	/*
	 * Constructs a cube primitive
	 * <returns>Holding pointer to cube</return>
	*/
	static gold_unique_ptr<gold_cube> create();

	/*
	* Draw cube in world
	* <param name="camera">Camera to use as reference for drawing</param>
	*/
	virtual void render(const gold_camera *camera) override;
};