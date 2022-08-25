#include "entity.h"

#include "gold/camera.h"
#include "gold/graphics/model/factory.h"
#include "gold/util/time.h"

gold_entity::gold_entity(gold_unique_ptr<gold_model> &&model) : model(std::move(model))
{
}

void gold_entity::update(const gold_camera *camera)
{
	/*if (velocity.x)
	velocity.x = velocity.x > 0.f ? std::min(velocity.x - .1f * weight * gold_delta_time, 0.f) :-std::max(velocity.x +
	.1f * weight * gold_delta_time, 0.f);*/
	if (has_gravity && !is_colliding_with_anything())
		velocity.y -= .1f * gold_delta_time;
	/* else
	    velocity.y = velocity.y > 0.f ? std::min(velocity.y - .1f * weight * gold_delta_time, 0.f)
	                                  : -std::max(velocity.y + .1f * weight * gold_delta_time, 0.f);
	if (velocity.z)
	    velocity.z = velocity.z > 0.f ? std::min(velocity.z - .1f * weight * gold_delta_time, 0.f)
	                                  : -std::max(velocity.z + .1f * weight * gold_delta_time, 0.f);*/

	move(velocity);

	if (camera->get_parent() != this)
		model->render(camera);
}

gold_weak_ptr<gold_model> gold_entity::get_model() const
{
	return model.get_weak_ptr();
}

void gold_entity::set_position(const gold_vector3 &position)
{
	model->set_position(position);
}

const gold_vector3 &gold_entity::get_position() const
{
	return model->get_position();
}

void gold_entity::set_rotation(const gold_vector3 &rotation)
{
	model->set_rotation(rotation);
}

const gold_vector3 &gold_entity::get_rotation() const
{
	return model->get_rotation();
}

void gold_entity::add_velocity(const gold_vector3 &velocity)
{
	this->velocity += velocity;
}

void gold_entity::add_velocity_relative(const gold_vector3 &velocity)
{
	const auto &forward = get_forward();
	const auto &up      = get_up();
	const auto &left    = get_left();

	this->velocity += (forward * velocity.x) + (up * velocity.y) + (left * velocity.z);
}

void gold_entity::reset_velocity()
{
	velocity = {};
}

const gold_vector3 &gold_entity::get_velocity() const
{
	return velocity;
}

gold_vector3 gold_entity::get_up() const
{
	const auto &matrix   = model->get_model_matrix();
	const auto &inverted = glm::inverse(matrix);
	return { inverted[1][0], inverted[1][1], inverted[1][2] };
}

gold_vector3 gold_entity::get_forward() const
{
	const auto &matrix   = model->get_model_matrix();
	const auto &inverted = glm::inverse(matrix);
	return { inverted[2][0], inverted[2][1], inverted[2][2] };
}

gold_vector3 gold_entity::get_left() const
{
	const auto &matrix   = model->get_model_matrix();
	const auto &inverted = glm::inverse(matrix);
	return { inverted[0][0], inverted[0][1], inverted[0][2] };
}

void gold_entity::move(const gold_vector3 &offset)
{
	auto old_position = get_position();
	set_position(old_position + offset);

	if (is_colliding_with_anything())
	{
		set_position(old_position);
		velocity = {};
	}
}

void gold_entity::move_relative(const gold_vector3 &offset)
{
	const auto &forward = get_forward();
	const auto &up      = get_up();
	const auto &left    = get_left();

	auto old_position   = get_position();
	set_position(old_position + (forward * offset.x) + (up * offset.y) + (left * offset.z));

	if (is_colliding_with_anything())
	{
		set_position(old_position);
		velocity = {};
	}
}

bool gold_entity::is_colliding_with(const gold_entity &entity, const gold_vector3 &offset) const
{
	return model->is_colliding_with(*entity.model, offset);
}

bool gold_entity::is_colliding_with_anything(const gold_vector3 &offset) const
{
	for (const auto &[name, entity] : gold_factory.get_all_entities())
	{
		if (entity.handle() == this)
			continue;

		if (is_colliding_with(*entity, offset))
			return true;
	}

	return false;
}

bool gold_entity::is_colliding_with_any_trigger(const gold_vector3 &offset) const
{
	for (const auto &[name, entity] : gold_factory.get_all_entities())
	{
		if (entity.handle() == this || !entity->model->is_trigger)
			continue;

		if (is_colliding_with(*entity, offset))
			return true;
	}

	return false;
}

bool gold_entity::is_on_ground() const
{
	return is_colliding_with_anything({ 0.f, -.1f, 0.f });
}