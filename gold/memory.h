#pragma once

#include "gold/util/vector.h"

#include <corecrt.h>

#include <cstdint>
#include <map>

#define GOLD_MEMORY_DEFAULT_POOL_SIZE 1024 * 1024 * 100 // 100 MB

class gold_memory
{
	void *pool       = nullptr;
	size_t pool_size = 0;

	struct block
	{
		std::uintptr_t ptr;
		size_t size;

		bool operator==(std::uintptr_t ptr) const
		{
			return this->ptr == ptr;
		}
	};
	std::map<std::uintptr_t, size_t> allocated_blocks;

  public:
	gold_memory(size_t pool_size);

	~gold_memory();

	gold_memory(gold_memory &)            = delete;

	gold_memory &operator=(gold_memory &) = delete;

  private:
	void *_allocate(size_t size);
	void _deallocate(void *block);

  public:
	template <typename t> inline t *allocate(size_t size = sizeof(t))
	{
		return reinterpret_cast<t *>(_allocate(size));
	}

	template <typename t = void> inline void deallocate(t *block)
	{
		_deallocate(reinterpret_cast<void *>(block));
	}
};

inline gold_memory gold_memory = { GOLD_MEMORY_DEFAULT_POOL_SIZE };

template <typename t> inline t *gold_global_allocate(size_t size)
{
	return gold_memory.allocate<t>(size);
}

template <typename t> inline void gold_global_deallocate(t *block)
{
	gold_memory.deallocate(block);
}