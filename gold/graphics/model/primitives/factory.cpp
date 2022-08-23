#include "factory.h"

gold_cube *gold_factory::create_cube(const gold_vector3 &pos, const gold_string &name, bool is_plane)
{
	if (objects_pool.contains(name))
	{
		LOG_ERROR("Couldn't create object " << name << ": already exists!");
		return nullptr;
	}

	objects_pool[name] = gold_unique_ptr<gold_cube>::create(pos, is_plane);
	return objects_pool.at(name).handle();
}

gold_cube *gold_factory::get_object(const gold_string &name) const
{
	return objects_pool.contains(name) ? objects_pool.at(name).handle() : nullptr;
}

const auto &gold_factory::get_objects() const
{
	return objects_pool;
}