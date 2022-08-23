#pragma once

#include "gold/graphics/model/model.h"
#include "gold/memory.h"

class gold_vector3;
class gold_camera;

class gold_skybox
{
  private:
	gold_unique_ptr<gold_model> model;

  public:
	gold_skybox();

	void render(const gold_camera *camera) const;
	gold_model *get_model() const;
};