#pragma once

#include "gold/graphics/model/primitives/cube.h"
#include "gold/memory.h"
#include "gold/util/macros.h"
#include "gold/util/vector3.h"

#include <string>
#include <string_view>
#include <unordered_map>
#include <memory>

class gold_factory
{
	std::unordered_map<std::string, std::unique_ptr<gold_cube>> objects_pool;

  public:
	gold_cube *create_cube(const gold_vector3 &pos, const std::string &name, bool is_plane = false);

	gold_cube *get_object(const std::string &name) const;

	const auto &get_objects() const;
};

inline gold_factory gold_factory;