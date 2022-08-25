#pragma once

#include "gold/memory.h"
#include "gold/util/vector3.h"

#include <glm/glm.hpp>

class gold_graphicsdevice;
class gold_entity;

/*
* Gold camera
*/
class gold_camera
{
  private:
	gold_graphicsdevice *graphics_device = nullptr;
	gold_vector3 eye;
	gold_vector3 at;
	gold_vector3 up { 0.f, 1.f, 0.f };
	float fov_horizontal = 0.f, fov_vertical = 0.f;

	gold_entity* parent = nullptr;

	float yaw = 0.f;
	float pitch = 0.f;

  public:
	/*
	* Whether the camera should be rotatable through mouse movement
	*/
	bool user_rotatable = false;

	/*
	* Constructor
	* <param name="graphics_device">Current graphics device</param>
	*/
	gold_camera(gold_graphicsdevice *graphics_device);

	/*
	* Camera update, call this once a frame
	*/
	void update();

	/*
	* Set view position
	* <param name="eye">View position</param>
	*/
	void set_eye(const gold_vector3 &eye);
	/*
	* <returns>View position</param>
	*/
	gold_vector3 get_eye() const;

	/*
	 * Set target view position
	 * <param name="at">Target view position</param>
	 */
	void set_look_at(const gold_vector3 &at);
	/*
	 * <returns>Target view position</param>
	 */
	gold_vector3 get_look_at() const;

	/*
	 * Set up vector
	 * <param name="up">Up vector</param>
	 */
	void set_up(const gold_vector3 &up);
	/*
	 * <returns>Up vector</param>
	 */
	gold_vector3 get_up() const;
	/*
	 * <returns>Forward vector</param>
	 */
	gold_vector3 get_forward() const;
	/*
	 * <returns>Left vector</param>
	 */
	gold_vector3 get_left() const;

	/*
	 * Set field of view
	 * <param name="fov_horizontal">Horizontal field of view</param>
	 * <param name="fov_vertical">Vertical field of view</param>
	 */
	void set_fov(float fov_horizontal, float fov_vertical);
	/*
	 * <returns>Horizontal field of view</param>
	 */
	float get_fov_horizontal() const;
	/*
	 * <returns>Vertical field of view</param>
	 */
	float get_fov_vertical() const;

	/*
	 * Move camera
	 * <param name="move">Move vector</param>
	 */
	void move(const gold_vector3 &move);
	/*
	 * Move camera relatively to camera rotation
	 * <param name="move">Move vector</param>
	 */
	void move_relative(const gold_vector3 &move);

	/*
	 * <returns>Perspective matrix</param>
	 */
	glm::highp_mat4 get_perspective() const;
	/*
	 * <returns>View matrix</param>
	 */
	glm::highp_mat4 get_view() const;

	/*
	 * Attach camera to entity
	 * <param name="entity">Entity to attach to</param>
	 */
	void attach_to_entity(gold_entity* entity);
	/*
	 * Detach camera from current parent (if one exists)
	 */
	void detach_from_parent();
	/*
	 * <returns>Whether camera is attached to a entity</returns>
	 */
	bool has_parent() const;
	/*
	 * <returns>Entity camera is attached to, nullptr if none</returns>
	 */
	gold_entity* get_parent() const;

	/*
	 * Move parent relative to camera rotation
	 * <param name="move">Move vector</param>
	 * <param name="ignore_up">Whether to ignore up axis</param>
	 */
	void move_parent_relatively(const gold_vector3 &move, bool ignore_up = false);
};