#pragma once

#include "gold/util/vector3.h"

#include <glm/glm.hpp>

#define _NODISCARD [[nodiscard]]

class gold_graphicsdevice;

class gold_camera
{
  private:
	gold_graphicsdevice *graphics_dev = nullptr;
	gold_vector3 eye {};
	gold_vector3 at {};
	gold_vector3 up {};
	float fov_horizontal = 0.f, fov_vertical = 0.f;

  public:
	gold_camera(gold_graphicsdevice *graphics_dev);

	void update();

	void set_eye(const gold_vector3 &eye);
	_NODISCARD const gold_vector3 &get_eye() const;

	void set_look_at(const gold_vector3 &at);
	_NODISCARD const gold_vector3 &get_look_at() const;

	void set_up(const gold_vector3 &up);
	_NODISCARD const gold_vector3 &get_up() const;

	void set_fov(float fov_horizontal, float fov_vertical);
	_NODISCARD float get_fov_horizontal() const;
	_NODISCARD float get_fov_vertical() const;

	void move(const gold_vector3 &move);

	_NODISCARD glm::highp_mat4 get_perspective() const;
	_NODISCARD glm::highp_mat4 get_view() const;
};