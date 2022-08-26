#include "camera.h"

#include "gold/entity.h"
#include "gold/graphics/graphics_device.h"
#include "gold/util/matrix.h"
#include "gold/util/util.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define NPLANE .5f
#define FPLANE 1000.f

gold_camera::gold_camera(gold_graphicsdevice *graphics_device) : graphics_device(graphics_device)
{
}

void gold_camera::update()
{
	if (!user_rotatable)
		return;

	const auto &cursor_dist = graphics_device->get_last_cursor_distance();
	const auto &eye         = get_eye();

	yaw += cursor_dist.x * 0.05f;
	pitch = std::max(-89.f, std::min(pitch - cursor_dist.y * 0.05f, 89.f));

	gold_vector3 new_rot;
	new_rot.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	new_rot.y = sin(glm::radians(pitch));
	new_rot.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	at = { eye.x + new_rot.x, eye.y + new_rot.y, eye.z + new_rot.z };
}

void gold_camera::set_eye(const gold_vector3 &eye)
{
	this->eye = eye;
}

gold_vector3 gold_camera::get_eye() const
{
	return parent ? parent->get_position() + eye : eye;
}

void gold_camera::set_look_at(const gold_vector3 &at)
{
	this->at = at;
}

gold_vector3 gold_camera::get_look_at() const
{
	return parent ? parent->get_position() + at : at;
}

void gold_camera::set_up(const gold_vector3 &up)
{
	this->up = up;
}

gold_vector3 gold_camera::get_up() const
{
	const auto &view     = get_view();
	const auto &inverted = glm::inverse(view);
	return { inverted[1][0], inverted[1][1], inverted[1][2] };
}

gold_vector3 gold_camera::get_forward() const
{
	const auto &view     = get_view();
	const auto &inverted = glm::inverse(view);
	return { inverted[2][0], inverted[2][1], inverted[2][2] };
}

gold_vector3 gold_camera::get_left() const
{
	const auto &view     = get_view();
	const auto &inverted = glm::inverse(view);
	return { inverted[0][0], inverted[0][1], inverted[0][2] };
}

void gold_camera::set_fov(float fov_horizontal, float fov_vertical)
{
	this->fov_horizontal = fov_horizontal;
	this->fov_vertical   = fov_vertical;
}

float gold_camera::get_fov_horizontal() const
{
	return fov_horizontal;
}

float gold_camera::get_fov_vertical() const
{
	return fov_vertical;
}

void gold_camera::move(const gold_vector3 &move)
{
	eye += move;
	at += move;
}

void gold_camera::move_relative(const gold_vector3 &move)
{
	const auto &forward = get_forward();
	const auto &up      = get_up();
	const auto &left    = get_left();

	auto offset         = (forward * move.x) + (up * move.y) + (left * move.z);
	eye += offset;
	at += offset;
}

glm::highp_mat4 gold_camera::get_perspective() const
{
	return glm::perspective(glm::radians(fov_horizontal), fov_vertical, NPLANE, FPLANE);
}

glm::highp_mat4 gold_camera::get_view() const
{
	const auto &eye = get_eye();
	return glm::lookAt(glm::vec3(eye.x, eye.y, eye.z), glm::vec3(at.x, at.y, at.z), glm::vec3(up.x, up.y, up.z));
}

void gold_camera::attach_to_entity(gold_entity* entity)
{
	parent = entity;
}

void gold_camera::detach_from_parent()
{
	parent = nullptr;
}

bool gold_camera::has_parent() const
{
	return parent;
}

gold_entity *gold_camera::get_parent() const
{
	return parent;
}

void gold_camera::move_parent_relatively(const gold_vector3 &move, bool ignore_up)
{
	if (!parent)
		return;

	const auto &forward = get_forward();
	const auto &up      = get_up();
	const auto &left    = get_left();

	auto offset = gold_vector3(forward.x * move.x, forward.y * move.x * !ignore_up, forward.z * move.x) + (up * move.y)
	            + (left * move.z);
	parent->move(offset);
}