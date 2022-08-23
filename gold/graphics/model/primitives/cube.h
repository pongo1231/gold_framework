#pragma once

#include "gold/graphics/model/model.h"
#include "gold/memory.h"

class gold_cube : public gold_model
{
	gold_cube(gold_unique_ptr<gold_mesh> &&mesh, gold_unique_ptr<gold_shader_program> &&shader_program);

  public:
	static gold_unique_ptr<gold_cube> create();

	virtual void render(const gold_camera *camera) override;
};