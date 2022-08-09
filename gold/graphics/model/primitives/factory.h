#pragma once

#include "gold/graphics/model/primitives/cube.h"
#include "gold/util/macros.h"

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

class gold_factory
{
	std::unordered_map<std::string, std::unique_ptr<gold_cube>> objects_pool;

  public:
	gold_cube *create_cube(const gold_vector3 &pos, const std::string &name, bool is_plane = false);

	_NODISCARD gold_cube *get_object(const std::string &name) const;

	_NODISCARD const auto &get_objects() const;
};

inline gold_factory gold_factory;