#pragma once

#include "gold/util/assert.h"

#define GOLD_VECTOR_ALLOC_BLOCK_ELEMENTCOUNT 16

template <typename t> t *gold_global_allocate(size_t size = sizeof(t));
template <typename t> void gold_global_deallocate(t *block);

template <typename t, void *(*allocator)(size_t) = gold_global_allocate<void>,
          void (*deallocator)(void *) = gold_global_deallocate<void>>
class gold_vector
{
	t *block                 = nullptr;
	size_t elements_cur_size = 0;
	size_t elements_size     = 0;

  public:
	gold_vector()
	{
		resize(GOLD_VECTOR_ALLOC_BLOCK_ELEMENTCOUNT);
	}

	gold_vector(t *block, size_t size)
	{
		this->block   = block;
		elements_size = size;
	}

	gold_vector(gold_vector &vector)
	{
		resize(vector.elements_size);
		memcpy(block, vector.block, vector.size());
		elements_size     = vector.elements_size;
		elements_cur_size = vector.elements_cur_size;
	}

	~gold_vector()
	{
		deallocator(block);
	}

	t &operator[](size_t index) const
	{
		return block[index];
	}

	t *begin() const
	{
		return block;
	}

	t *end() const
	{
		return block + elements_cur_size;
	}

	void resize(size_t new_size)
	{
		t *new_block = allocator(new_size);
		if (!new_block)
			gold_assert("gold_vector::resize memory allocation failed!");

		int new_elements_cur_size = 0;
		if (block)
		{
			for (size_t i = 0; i < std::min(elements_cur_size, new_size); i++)
			{
				if (new_elements_cur_size == elements_size)
					break;

				new_block[new_elements_cur_size++] = block[i];
			}
			deallocator(block);
		}

		block             = new_block;
		elements_cur_size = new_elements_cur_size;
		elements_size     = new_size;
	}

	void push(const t &element)
	{
		if (elements_cur_size >= elements_size)
			resize(elements_size + GOLD_VECTOR_ALLOC_BLOCK_ELEMENTCOUNT);

		block[elements_cur_size++] = element;
	}

	void emplace(auto &&...args)
	{
		push({ args... });
	}

	void erase_index(size_t index)
	{
		if (index >= elements_cur_size)
			gold_assert("gold_vector::erase_index index too large!");

		for (size_t i = index + 1; i < elements_cur_size; i++)
			block[i - 1] = block[i];

		elements_cur_size--;
	}

	void erase_element(t *element)
	{
		size_t index =
		    (reinterpret_cast<std::uintptr_t>(element) - reinterpret_cast<std::uintptr_t>(block)) / sizeof(t);
		if (index % sizeof(t) > 0)
			gold_assert("gold_vector::erase_address invalid address!");

		erase_index(index);
	}

	void pop()
	{
		if (elements_cur_size > 0)
			erase_index(elements_cur_size - 1);
	}

	size_t size() const
	{
		return elements_cur_size;
	}

	size_t allocated_size() const
	{
		return elements_size;
	}

	t *data() const
	{
		return block;
	}

	t *first() const
	{
		return begin();
	}

	t *last() const
	{
		return end() - 1;
	}

	t *find(const auto &value) const
	{
		for (size_t i = 0; i < elements_cur_size; i++)
		{
			if (block[i] == value)
				return block + i;
		}

		return nullptr;
	}
};