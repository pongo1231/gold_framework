#pragma once

#include "gold/memory.h"
#include "gold/util/vector3.h"

#include <glm/glm.hpp>

class gold_graphicsdevice;
class gold_entity;

class gold_camera
{
  private:
	gold_graphicsdevice *graphics_device = nullptr;
	gold_vector3 eye;
	gold_vector3 at;
	gold_vector3 up { 0.f, 1.f, 0.f };
	float fov_horizontal = 0.f, fov_vertical = 0.f;

	gold_entity* parent = nullptr;

  public:
	gold_camera(gold_graphicsdevice *graphics_device);

	void update();

	void set_eye(const gold_vector3 &eye);
	gold_vector3 get_eye() const;

	void set_look_at(const gold_vector3 &at);
	gold_vector3 get_look_at() const;

	void set_up(const gold_vector3 &up);
	gold_vector3 get_up() const;
	gold_vector3 get_forward() const;
	gold_vector3 get_left() const;

	void set_fov(float fov_horizontal, float fov_vertical);
	float get_fov_horizontal() const;
	float get_fov_vertical() const;

	void move(const gold_vector3 &move);
	void move_relative(const gold_vector3 &move);

	glm::highp_mat4 get_perspective() const;
	glm::highp_mat4 get_view() const;

	void attach_to_entity(gold_entity* entity);
	void detach_from_parent();
	bool has_parent() const;
	gold_entity* get_parent() const;

	void move_parent_relatively(const gold_vector3 &move, bool ignore_up = false);
};