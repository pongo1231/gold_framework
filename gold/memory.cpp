#include "memory.h"

#include <cstdlib>

gold_memory::gold_memory(size_t pool_size) : pool(malloc(pool_size)), pool_size(pool_size)
{
}

gold_memory::~gold_memory()
{
	free(pool);
}

void *gold_memory::_allocate(size_t size)
{
	std::lock_guard lock(mutex);

	auto cur_address = reinterpret_cast<std::uintptr_t>(pool);
	for (const auto &[alloc_ptr, alloc_size] : allocated_blocks)
	{
		if (cur_address + size < alloc_ptr)
			break;
		else
			cur_address = alloc_ptr + alloc_size;
	}

	auto pool_end = reinterpret_cast<std::uintptr_t>(pool) + pool_size;
	if (cur_address == pool_end || cur_address + size >= pool_end)
		gold_assert(
		    "gold_memory::assert couldn't allocate memory (dynamic additional pool allocating not implemented yet)");

	allocated_blocks[cur_address] = size;

	return reinterpret_cast<void *>(cur_address);
}

void gold_memory::_deallocate(void *block)
{
	std::lock_guard lock(mutex);

	auto result = allocated_blocks.find(reinterpret_cast<std::uintptr_t>(block));
	if (result == allocated_blocks.end())
		gold_assert("gold_memory::deallocate couldn't find given block");

	allocated_blocks.erase(result);
}