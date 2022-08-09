#include "factory.h"

gold_cube *gold_factory::create_cube(const gold_vector3 &pos, const std::string &name, bool is_plane)
{
	if (objects_pool.contains(name))
	{
		LOG_ERROR("Couldn't create object " << name << ": already exists!");
		return nullptr;
	}

	objects_pool[name] = std::make_unique<gold_cube>(pos, is_plane);
	return objects_pool.at(name).get();
}

gold_cube *gold_factory::get_object(const std::string &name) const
{
	return objects_pool.contains(name) ? objects_pool.at(name).get() : nullptr;
}

const auto &gold_factory::get_objects() const
{
	return objects_pool;
}