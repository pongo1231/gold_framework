#pragma once

#include "gold/graphics/model/model.h"
#include "gold/graphics/model/model_type.h"
#include "gold/graphics/model/primitives/cube.h"
#include "gold/graphics/model/primitives/skybox.h"
#include "gold/memory.h"
#include "gold/util/macros.h"
#include "gold/util/string.h"
#include "gold/util/vector3.h"

#include <string_view>
#include <unordered_map>

class gold_factory
{
	static inline auto objects_pool_hash = [](const gold_string &key)
	{
		// Taken from https://stackoverflow.com/a/15811185
		size_t hash = 0;
		for (size_t i = 0; i < key.length(); i++)
		{
			hash += (71 * hash + key[i]) % 5;
		}
		return hash;
	};

	std::unordered_map<gold_string, gold_unique_ptr<gold_model>, decltype(objects_pool_hash)> objects_pool;

  public:
	template <gold_model_type model_type, class model = gold_model>
	model *create_model(const gold_string &name, std::string_view filename = {})
	{
		if (objects_pool.contains(name))
		{
			LOG_ERROR("Couldn't create object " << name << ": already exists!");
			return nullptr;
		}

		switch (model_type)
		{
		case gold_model_type::obj_model:
			objects_pool[name] = gold_model::load_from_obj(filename);
			break;
		case gold_model_type::cube:
			objects_pool[name] = gold_unique_ptr<gold_model>(gold_cube::create());
			break;
		case gold_model_type::skybox:
			objects_pool[name] = gold_unique_ptr<gold_model>(gold_skybox::create());
			break;
		}

		return reinterpret_cast<model *>(objects_pool.at(name).handle());
	}

	template <class model = gold_model> model *get_object(const gold_string &name) const
	{
		return objects_pool.contains(name) ? reinterpret_cast<model *>(objects_pool.at(name).handle()) : nullptr;
	}

	const auto &get_objects() const
	{
		return objects_pool;
	}
};

inline gold_factory gold_factory;