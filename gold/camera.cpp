#include "camera.h"

#include "gold/graphics/graphics_device.h"
#include "gold/util/matrix.h"
#include "gold/util/util.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define NPLANE .5f
#define FPLANE 100.f

gold_camera::gold_camera(gold_graphicsdevice *graphics_dev) : graphics_dev(graphics_dev)
{
}

void gold_camera::update()
{
}

void gold_camera::set_eye(const gold_vector3 &eye)
{
	this->eye = eye;
}

_NODISCARD const gold_vector3 &gold_camera::get_eye() const
{
	return eye;
}

void gold_camera::set_look_at(const gold_vector3 &at)
{
	this->at = at;
}

_NODISCARD const gold_vector3 &gold_camera::get_look_at() const
{
	return at;
}

void gold_camera::set_up(const gold_vector3 &up)
{
	this->up = up;
}

_NODISCARD const gold_vector3 &gold_camera::get_up() const
{
	return up;
}

void gold_camera::set_fov(float fov_horizontal, float fov_vertical)
{
	this->fov_horizontal = fov_horizontal;
	this->fov_vertical   = fov_vertical;
}

_NODISCARD float gold_camera::get_fov_horizontal() const
{
	return fov_horizontal;
}

_NODISCARD float gold_camera::get_fov_vertical() const
{
	return fov_vertical;
}

void gold_camera::move(const gold_vector3 &move)
{
	eye += move;
	at += move;
}

_NODISCARD glm::highp_mat4 gold_camera::get_perspective() const
{
	return glm::perspective(glm::radians(fov_horizontal), fov_vertical, NPLANE, FPLANE);
}

_NODISCARD glm::highp_mat4 gold_camera::get_view() const
{
	return glm::lookAt(glm::vec3(eye.x, eye.y, eye.z), glm::vec3(at.x, at.y, at.z), glm::vec3(up.x, up.y, up.z));
}