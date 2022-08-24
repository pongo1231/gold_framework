#include "camera.h"

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
}

void gold_camera::set_eye(const gold_vector3 &eye)
{
	this->eye = eye;
}

const gold_vector3 &gold_camera::get_eye() const
{
	return eye;
}

void gold_camera::set_look_at(const gold_vector3 &at)
{
	this->at = at;
}

const gold_vector3 &gold_camera::get_look_at() const
{
	return at;
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
	return glm::lookAt(glm::vec3(eye.x, eye.y, eye.z), glm::vec3(at.x, at.y, at.z), glm::vec3(up.x, up.y, up.z));
}