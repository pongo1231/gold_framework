#pragma once

#include "gold/graphics/model/model.h"

#include <memory>

#define _NODISCARD [[nodiscard]]

class gold_vector3;
class gold_camera;

class gold_cube
{
  private:
	std::unique_ptr<gold_model> model;
	float specular_multiplier = .5f;
	float shininess           = 64.f;

  public:
	gold_cube(const gold_vector3 &pos, bool is_plane = false);

	void render(const gold_camera *camera) const;
	_NODISCARD gold_model *get_model() const;

	void set_specular_multiplier(float spec_multiplier);
	void set_shininess(float shininess);
};