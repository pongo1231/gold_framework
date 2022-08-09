#pragma once

#include "gold/util/assert.h"

#include <corecrt.h>

#include <cstdint>
#include <map>
#include <mutex>

#define GOLD_MEMORY_DEFAULT_POOL_SIZE 1024 * 1024 * 100 // 100 MB
#define GOLD_VECTOR_ALLOC_BLOCK_ELEMENTCOUNT 16ull

void *gold_global_allocate(size_t size);
void gold_global_deallocate(void *block);

template <typename t, void *(*allocator)(size_t) = gold_global_allocate,
          void (*deallocator)(void *) = gold_global_deallocate>
class gold_vector
{
	t *block                 = nullptr;
	size_t elements_cur_size = 0;
	size_t elements_size     = 0;

  public:
	gold_vector(std::initializer_list<t> elements = {})
	{
		resize(std::max(elements.size(), GOLD_VECTOR_ALLOC_BLOCK_ELEMENTCOUNT));
		if (elements.size() > 0)
		{
			for (size_t i = 0; i < elements.size(); i++)
				block[i] = *(elements.begin() + i);
			elements_cur_size = elements.size();
		}
	}

	gold_vector &operator=(std::initializer_list<t> elements)
	{
		deallocate(block);
		block = nullptr;

		resize(std::max(elements.size(), GOLD_VECTOR_ALLOC_BLOCK_ELEMENTCOUNT));
		if (elements.size() > 0)
		{
			for (size_t i = 0; i < elements.size(); i++)
				block[i] = *(elements.begin() + i);
			elements_cur_size = elements.size();
		}
	}

	gold_vector(t *block, size_t size)
	{
		this->block   = block;
		elements_size = size;
	}

	gold_vector(const gold_vector &vector)
	{
		resize(vector.elements_size);
		for (const auto &element : vector)
			push(element);
	}

	gold_vector &operator=(const gold_vector &vector)
	{
		clear();
		resize(vector.elements_size);
		for (const auto &element : vector)
			push(element);

		return *this;
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
		auto new_block = reinterpret_cast<t *>(allocator(new_size * sizeof(t)));
		if (!new_block)
			gold_assert("gold_vector::resize memory allocation failed!");

		size_t new_elements_cur_size = 0;
		if (block)
		{
			for (; new_elements_cur_size < std::min(elements_cur_size, new_size); new_elements_cur_size++)
				new_block[new_elements_cur_size] = block[new_elements_cur_size];
			deallocator(block);
		}

		block             = new_block;
		elements_cur_size = new_elements_cur_size;
		elements_size     = new_size;
	}

	void clear()
	{
		deallocator(block);
		block = nullptr;
		resize(GOLD_VECTOR_ALLOC_BLOCK_ELEMENTCOUNT);
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

class gold_memory
{
	void *pool       = nullptr;
	size_t pool_size = 0;
	std::mutex mutex;

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

	gold_memory(gold_memory &)           = delete;

	gold_memory operator=(gold_memory &) = delete;

  private:
	void *_allocate(size_t size);
	void _deallocate(void *block);

  public:
	inline void *allocate(size_t size)
	{
		return _allocate(size);
	}

	inline void deallocate(void *block)
	{
		_deallocate(block);
	}
};

inline gold_memory gold_memory = { GOLD_MEMORY_DEFAULT_POOL_SIZE };

inline void *gold_global_allocate(size_t size)
{
	return gold_memory.allocate(size);
}

inline void gold_global_deallocate(void *block)
{
	gold_memory.deallocate(block);
}

template <typename t, void *(*allocator)(size_t) = gold_global_allocate,
          void (*deallocator)(void *) = gold_global_deallocate>
class gold_unique_ptr
{
	t *data                 = nullptr;
	bool has_data_ownership = true;

  public:
	gold_unique_ptr(t *data = nullptr) : data(data)
	{
	}

	~gold_unique_ptr()
	{
		if (has_data_ownership && data)
			deallocator(data);
	}

	gold_unique_ptr(const gold_unique_ptr &)            = delete;

	gold_unique_ptr &operator=(const gold_unique_ptr &) = delete;

	gold_unique_ptr(const gold_unique_ptr &&ptr) : gold_unique_ptr(ptr.data)
	{
	}

	gold_unique_ptr &operator=(gold_unique_ptr &&ptr)
	{
		if (data)
			deallocator(data);

		data                   = ptr.data;
		ptr.has_data_ownership = false;

		return *this;
	}

	operator bool() const
	{
		return data;
	}

	t &operator*() const
	{
		return *data;
	}

	t *operator->() const
	{
		return data;
	}

	t *handle() const
	{
		return data;
	}

	static gold_unique_ptr create(auto &&...args)
	{
		auto alloc = reinterpret_cast<t *>(allocator(sizeof(t)));
		std::construct_at(alloc, args...);
		return gold_unique_ptr(alloc);
	}
};

template <typename t, void *(*allocator)(size_t) = gold_global_allocate,
          void (*deallocator)(void *) = gold_global_deallocate>
class gold_ref_ptr
{
	t *data          = nullptr;
	size_t *refcount = nullptr;

  public:
	gold_ref_ptr(t *data = nullptr) : data(data), refcount(reinterpret_cast<size_t *>(allocator(sizeof(size_t))))
	{
		(*refcount)++;
	}

	~gold_ref_ptr()
	{
		if (!--(*refcount))
		{
			deallocator(refcount);
			if (data)
				deallocator(data);
		}
	}

	gold_ref_ptr(const gold_ref_ptr &ptr) : data(ptr.data), refcount(ptr.refcount)
	{
		(*refcount)++;
	}

	gold_ref_ptr &operator=(const gold_ref_ptr &ptr)
	{
		data     = ptr.data;
		refcount = ptr.refcount;
		(*refcount)++;

		return *this;
	}

	operator bool() const
	{
		return data;
	}

	t &operator*() const
	{
		return *data;
	}

	t *operator->() const
	{
		return data;
	}

	t *handle() const
	{
		return data;
	}

	static gold_ref_ptr create(auto &&...args)
	{
		auto alloc = reinterpret_cast<t *>(allocator(sizeof(t)));
		std::construct_at(alloc, args...);
		return gold_ref_ptr(alloc);
	}
};