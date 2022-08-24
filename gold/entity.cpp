#include "entity.h"

#include "gold/camera.h"

gold_entity::gold_entity(gold_unique_ptr<gold_model> &&model) : model(std::move(model))
{
}

void gold_entity::update(const gold_camera *camera)
{
	model->set_position(position);
	model->set_rotation(rotation);
	model->render(camera);
}

gold_weak_ptr<gold_model> gold_entity::get_model() const
{
	return model.get_weak_ptr();
}

void gold_entity::set_position(const gold_vector3 &position)
{
	this->position = position;
}

const gold_vector3 &gold_entity::get_position() const
{
	return position;
}

void gold_entity::set_rotation(const gold_vector3 &rotation)
{
	this->rotation = rotation;
}

const gold_vector3 &gold_entity::get_rotation() const
{
	return rotation;
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
	position += offset;
}

void gold_entity::move_relative(const gold_vector3 &offset)
{
	const auto &forward = get_forward();
	const auto &up      = get_up();
	const auto &left    = get_left();

	position += (forward * offset.x) + (up * offset.y) + (left * offset.z);
}