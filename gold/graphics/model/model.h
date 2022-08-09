#pragma once

#include <memory>

#include "gold/util/vector3.h"

#define _NODISCARD [[nodiscard]]

class gold_mesh;
class gold_shader_program;
class gold_camera;
class gold_texture;

class gold_model
{
  private:
	const std::shared_ptr<gold_mesh> mesh;
	const std::shared_ptr<gold_shader_program> shader_program;
	gold_vector3 position;
	float angle = 0.f;
	gold_vector3 rotation { 1.f, 1.f, 1.f };
	gold_vector3 m_scale { 1.f, 1.f, 1.f };
	std::shared_ptr<gold_texture> texture;

  public:
	gold_model(const std::shared_ptr<gold_mesh> mesh, const std::shared_ptr<gold_shader_program> shader_program);

	void render(const gold_camera *camera) const;

	_NODISCARD const std::shared_ptr<gold_shader_program> get_shader_program() const;

	void set_texture(const std::shared_ptr<gold_texture> texture);

	const gold_vector3 &get_pos() const;
	void set_pos(const gold_vector3 &pos);
	const gold_vector3 &get_rotation() const;
	float get_angle() const;
	void set_rotation(float angle, const gold_vector3 &rot, bool in_radians = false);
	const gold_vector3 &get_scale() const;
	void set_scale(const gold_vector3 &scale);
};