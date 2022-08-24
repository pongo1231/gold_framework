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