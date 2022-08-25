#pragma once

#include "gold/graphics/model/model.h"
#include "gold/memory.h"

/*
* Skybox Primitive
*/
class gold_skybox : public gold_model
{
	/*
	* Constructs a skybox
	* <param name="mesh">Holding pointer for mesh (use std::move)</param>
	* <param name="shader_program">Holding pointer for shader program (use std::move)</param>
	*/
	gold_skybox(gold_unique_ptr<gold_mesh> &&mesh, gold_unique_ptr<gold_shader_program> &&shader_program);

  public:
	/*
	* Constructs a skybox
	* <returns>Holding pointer to skybox object</return>
	*/
	static gold_unique_ptr<gold_skybox> create();

	/*
	* Draw skybox in world
	* <param name="camera">Camera to use as reference for drawing</param>
	*/
	virtual void render(const gold_camera *camera) override;
};