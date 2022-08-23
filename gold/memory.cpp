#include "memory.h"

#include <cstdlib>

gold_memory::gold_memory(size_t pool_size) : pool(malloc(pool_size)), pool_size(pool_size)
{
	if (!pool)
		gold_assert("gold_memory::gold_memory couldn't allocate system memory");
}

gold_memory::~gold_memory()
{
	free(pool);
}

void *gold_memory::_allocate(size_t size)
{
	// std::lock_guard lock(mutex);

	auto pool_end              = reinterpret_cast<std::uintptr_t>(pool) + pool_size;

	std::uintptr_t new_address = allocated_blocks.empty() ? reinterpret_cast<std::uintptr_t>(pool) : 0;
	auto prev_it               = allocated_blocks.rbegin();
	for (auto it = allocated_blocks.rbegin(); it != allocated_blocks.rend(); it++)
	{
		const auto &alloc_ptr  = it->first;
		const auto &alloc_size = it->second;
		auto alloc_end         = alloc_ptr + alloc_size;

		if (it == allocated_blocks.rbegin())
		{
			if (alloc_end + size < pool_end)
			{
				new_address = alloc_end;
				break;
			}
		}
		else
		{
			const auto &prev_alloc_ptr = prev_it--->first;

			if (alloc_end + size < prev_alloc_ptr)
				new_address = alloc_end;
		}
	}

	if (!new_address)
		gold_assert(
		    "gold_memory::assert couldn't allocate memory (dynamic additional pool allocating not implemented yet)");

	allocated_blocks[new_address] = size;

	return reinterpret_cast<void *>(new_address);
}

void gold_memory::_deallocate(void *block)
{
	std::lock_guard lock(mutex);

	auto result = allocated_blocks.find(reinterpret_cast<std::uintptr_t>(block));
	if (result == allocated_blocks.end())
		gold_assert("gold_memory::deallocate couldn't find given block");

	allocated_blocks.erase(result);
}