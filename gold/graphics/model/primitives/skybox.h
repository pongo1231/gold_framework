#pragma once

#include "gold/graphics/model/model.h"

#define _NODISCARD [[nodiscard]]

class gold_vector3;
class gold_camera;

class gold_skybox
{
  private:
	std::unique_ptr<gold_model> model;

  public:
	gold_skybox();

	void render(const gold_camera *camera) const;
	_NODISCARD gold_model *get_model() const;
};