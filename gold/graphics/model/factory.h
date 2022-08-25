#pragma once

#include "gold/graphics/model/model.h"
#include "gold/graphics/model/model_type.h"
#include "gold/graphics/model/primitives/cube.h"
#include "gold/entity.h"
#include "gold/graphics/model/primitives/skybox.h"
#include "gold/memory.h"
#include "gold/util/macros.h"
#include "gold/util/string.h"
#include "gold/util/vector3.h"

#include <string_view>
#include <unordered_map>

/*
* Entity constructor
*/
class gold_factory
{
	/*
	* Hash function used for map
	* <param name="key">Key to use for hashing</param>
	*/
	static inline auto objects_pool_hash = [](const gold_string &key)
	{
		size_t hash = 0;
		for (size_t i = 0; i < key.length(); i++)
		{
			hash += key[i];
		}
		return hash;
	};

	/*
	* Pool holding entities
	* unordered_map for fast lookups using hashes
	*/
	std::unordered_map<gold_string, gold_unique_ptr<gold_entity>, decltype(objects_pool_hash)> entity_pool;

  public:
	/*
	* Constructs an entity
	* <param name="model_type">Type of entity to construct (obj_model, cube or skybox)</param>
	* <param name="name">Name to use as reference for entity</param>
	* <param name="filename">(for obj_model) Path to .obj file to use as model</param>
	* <returns>Raw pointer to created entity in pool, or nullptr if the name already exists</returns>
	*/
	template <gold_model_type model_type>
	gold_entity *create_entity(const gold_string &name, std::string_view filename = {})
	{
		if (entity_pool.contains(name))
		{
			LOG_ERROR("Couldn't create object " << name << ": already exists!");
			return nullptr;
		}

		gold_unique_ptr<gold_model> model;
		switch (model_type)
		{
		case gold_model_type::obj_model:
			model = gold_model::load_from_obj(filename);
			break;
		case gold_model_type::cube:
			model = gold_unique_ptr<gold_model>(gold_cube::create());
			break;
		case gold_model_type::skybox:
			model = gold_unique_ptr<gold_model>(gold_skybox::create());
			break;
		}

		if (!model)
			return nullptr;

		entity_pool[name] = gold_unique_ptr<gold_entity>::create(std::move(model));

		if (!entity_pool.contains(name))
			gold_assert("gold_weak_ptr::create_entity entity_pool does not contain new entry?");

		LOG("Created object " << name);

		return entity_pool.at(name).handle();
	}

	/*
	* Gets an entity inside pool using name
	* <param name="name">Reference name of entity</param>
	* <returns>Raw pointer to found entity in pool, or nullptr if invalid</returns>
	*/
	gold_entity* get_entity(const gold_string &name) const
	{
		return entity_pool.contains(name) ? entity_pool.at(name).handle() : nullptr;
	}

	/*
	 * <returns>All existing entities inside pool</returns>
	 */
	const auto &get_all_entities() const
	{
		return entity_pool;
	}
};

inline gold_factory gold_factory;