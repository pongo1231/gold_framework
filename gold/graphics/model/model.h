#pragma once

#include "gold/camera.h"
#include "gold/graphics/mesh.h"
#include "gold/graphics/shader_program.h"
#include "gold/memory.h"
#include "gold/util/vector3.h"

#include <string_view>

class gold_texture;

class gold_model
{
  private:
	gold_unique_ptr<gold_mesh> mesh;
	gold_unique_ptr<gold_shader_program> shader_program;
	gold_vector3 position = {};
	float angle           = 0.f;
	gold_vector3 rotation { 1.f, 1.f, 1.f };
	gold_vector3 m_scale { 1.f, 1.f, 1.f };
	gold_ref_ptr<gold_texture> texture;
	float specular_multiplier = .5f;
	float shininess           = 64.f;

  public:
	gold_model() = default;
	gold_model(gold_unique_ptr<gold_mesh> &&mesh, gold_unique_ptr<gold_shader_program> &&shader_program);

	virtual void render(const gold_camera *camera);

	const gold_shader_program *get_shader_program() const;

	void set_texture(gold_ref_ptr<gold_texture> texture);

	const gold_vector3 &get_pos() const;
	void set_position(const gold_vector3 &pos);

	const gold_vector3 &get_rotation() const;
	float get_angle() const;
	void set_rotation(float angle, const gold_vector3 &rot, bool in_radians = false);

	const gold_vector3 &get_scale() const;
	void set_scale(const gold_vector3 &scale);

	void set_specular_multiplier(float specular_multiplier);
	void set_shininess(float shininess);

	static gold_unique_ptr<gold_model> load_from_obj(std::string_view filename);
};