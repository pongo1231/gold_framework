#pragma once

#include "gold/graphics/model/model.h"

class gold_camera;

class gold_entity
{
	gold_unique_ptr<gold_model> model;
	gold_vector3 position;
	gold_vector3 rotation;

  public:
	gold_entity(gold_unique_ptr<gold_model> &&model);

	void update(const gold_camera *camera);

	gold_weak_ptr<gold_model> get_model() const;

	void set_position(const gold_vector3 &position);
	const gold_vector3 &get_position() const;

	void set_rotation(const gold_vector3 &rotation);
	const gold_vector3 &get_rotation() const;

	gold_vector3 get_up() const;
	gold_vector3 get_forward() const;
	gold_vector3 get_left() const;

	void move(const gold_vector3 &offset);
	void move_relative(const gold_vector3 &offset);
};