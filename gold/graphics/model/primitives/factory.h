#pragma once

#include "gold/graphics/model/primitives/cube.h"
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

	std::unordered_map<gold_string, gold_unique_ptr<gold_cube>, decltype(objects_pool_hash)> objects_pool;

  public:
	gold_cube *create_cube(const gold_vector3 &pos, const gold_string &name, bool is_plane = false);

	gold_cube *get_object(const gold_string &name) const;

	const auto &get_objects() const;
};

inline gold_factory gold_factory;