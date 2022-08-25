#pragma once

#include "gold/graphics/model/model.h"

class gold_camera;

/*
* Entity class
*/
class gold_entity
{
	gold_unique_ptr<gold_model> model;
	gold_vector3 velocity;

  public:
	/*
	* Whether this entity should be affected by gravity
	*/
	bool has_gravity = true;

  public:
	/*
	* Constructor
	* <param name="model">Model to use for entity</param>
	*/
	gold_entity(gold_unique_ptr<gold_model> &&model);

	/*
	 * Entity update
	 * <param name="camera">Camera to use for drawing</param>
	 */
	void update(const gold_camera *camera);

	/*
	* <returns>Entity model</returns>
	*/
	gold_weak_ptr<gold_model> get_model() const;

	/*
	 * Set entity position
	 * <param name="position">New position</param>
	 */
	void set_position(const gold_vector3 &position);
	/*
	 * <returns>Entity position</returns>
	 */
	const gold_vector3 &get_position() const;

	/*
	 * Set entity rotation
	 * <param name="rotation">New rotation</param>
	 */
	void set_rotation(const gold_vector3 &rotation);
	/*
	 * <returns>Entity rotation</returns>
	 */
	const gold_vector3 &get_rotation() const;

	/*
	 * Add velocity to entity
	 * Only works for y axis currently!
	 * <param name="velocity">Velocity to add</param>
	 */
	void add_velocity(const gold_vector3 &velocity);
	/*
	 * Add velocity to entity relative to entity rotation
	 * Only works for y axis currently!
	 * <param name="velocity">Velocity to add</param>
	 */
	void add_velocity_relative(const gold_vector3 &velocity);
	/*
	 * Reset velocity of entity
	 */
	void reset_velocity();
	/*
	 * <returns>Entity velocity</returns>
	 */
	const gold_vector3 &get_velocity() const;

	/*
	 * <returns>Entity up vector</returns>
	 */
	gold_vector3 get_up() const;
	/*
	 * <returns>Entity forward vector</returns>
	 */
	gold_vector3 get_forward() const;
	/*
	 * <returns>Entity left vector</returns>
	 */
	gold_vector3 get_left() const;

	/*
	 * Move entity
	 * <param name="offset">Move vector</param>
	 */
	void move(const gold_vector3 &offset);
	/*
	 * Move entity relative to entity rotation
	 * <param name="offset">Move vector</param>
	 */
	void move_relative(const gold_vector3 &offset);

	/*
	 * <param name="entity">Entity to check collisions against</param>
	 * <param name="offset">Position offset to apply to collision check</param>
	 * <returns>Whether entity is colliding with other entity</returns>
	 */
	bool is_colliding_with(const gold_entity &entity, const gold_vector3 &offset = {}) const;
	/*
	 * <param name="offset">Position offset to apply to collision check</param>
	 * <returns>Whether entity is colliding with any other non-trigger entity</returns>
	 */
	bool is_colliding_with_anything(const gold_vector3 &offset = {}) const;
	/*
	 * <param name="offset">Position offset to apply to collision check</param>
	 * <returns>Whether entity is colliding with any other trigger entity</returns>
	 */
	bool is_colliding_with_any_trigger(const gold_vector3 &offset = {}) const;

	/*
	 * <returns>Whether entity is not in the air</returns>
	 */
	bool is_on_ground() const;
};