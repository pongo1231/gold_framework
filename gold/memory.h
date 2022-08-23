#pragma once

#include "gold/util/assert.h"
#include "gold/util/macros.h"

#include <cstdint>
#include <map>

#define GOLD_MEMORY_DEFAULT_POOL_SIZE 1024 * 1024 * 102 // 100 MB
#define GOLD_VECTOR_ALLOC_BLOCK_ELEMENTCOUNT 32ull

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
		clear();
		resize(std::max(elements.size(), GOLD_VECTOR_ALLOC_BLOCK_ELEMENTCOUNT));
		if (elements.size() > 0)
		{
			for (size_t i = 0; i < elements.size(); i++)
				block[i] = *(elements.begin() + i);
			elements_cur_size = elements.size();
		}

		return *this;
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

		memset(new_block, 0, new_size * sizeof(t));

		size_t new_elements_cur_size = 0;
		if (block)
		{
			for (; new_elements_cur_size < std::min(elements_cur_size, new_size); new_elements_cur_size++)
			{
				new_block[new_elements_cur_size] = block[new_elements_cur_size];
				block[new_elements_cur_size]     = {};
			}
			deallocator(block);
		}

		block             = new_block;
		elements_cur_size = new_elements_cur_size;
		elements_size     = new_size;
	}

	void clear()
	{
		if (block)
		{
			for (auto &element : block)
				element = {};
			deallocator(block);
		}
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

		block[elements_cur_size--] = {};
	}

	t *erase_element(t *element)
	{
		size_t index =
		    (reinterpret_cast<std::uintptr_t>(element) - reinterpret_cast<std::uintptr_t>(block)) / sizeof(t);
		if (index % sizeof(t) > 0)
			gold_assert("gold_vector::erase_address invalid address!");

		erase_index(index);
		return element;
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

	bool empty() const
	{
		return elements_cur_size;
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
	gold_memory(size_t pool_size) : pool(malloc(pool_size)), pool_size(pool_size)
	{
		if (!pool)
			gold_assert("gold_memory::gold_memory couldn't allocate system memory");
	}

	~gold_memory()
	{
		free(pool);
	}

	gold_memory(const gold_memory &)           = delete;

	gold_memory operator=(const gold_memory &) = delete;

  private:
	void *_allocate(size_t size)
	{
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
			gold_assert("gold_memory::assert couldn't allocate memory (dynamic additional pool allocating not "
			            "implemented yet)");

		allocated_blocks[new_address] = size;

		return reinterpret_cast<void *>(new_address);
	}

	void _deallocate(void *block)
	{
		auto result = allocated_blocks.find(reinterpret_cast<std::uintptr_t>(block));
		if (result == allocated_blocks.end())
			gold_assert("gold_memory::deallocate couldn't find given block");

		allocated_blocks.erase(result);
	}

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
	t *data = nullptr;

  public:
	gold_unique_ptr(t *data = nullptr) : data(data)
	{
	}

	~gold_unique_ptr()
	{
		if (data)
			deallocator(data);
	}

	gold_unique_ptr(const gold_unique_ptr &)            = delete;

	gold_unique_ptr &operator=(const gold_unique_ptr &) = delete;

	template <typename ptrtype> gold_unique_ptr(gold_unique_ptr<ptrtype> &&ptr)
	{
		data = ptr.release();
	}

	template <typename ptrtype> gold_unique_ptr &operator=(gold_unique_ptr<ptrtype> &&ptr) noexcept
	{
		if (data && data != ptr.handle())
			deallocator(data);

		data = ptr.release();

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

	void set(t *new_data)
	{
		if (data)
			deallocator(data);
		data = new_data;
	}

	void clear()
	{
		if (data)
			deallocator(data);
		data = nullptr;
	}

	t *release()
	{
		auto old_data = data;
		data          = nullptr;
		return old_data;
	}

	t *handle() const
	{
		return data;
	}

	template <class... argtype> static gold_unique_ptr create(argtype &&...args)
	{
		auto alloc = reinterpret_cast<t *>(allocator(sizeof(t)));
		std::construct_at(alloc, std::forward<argtype>(args)...);
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
	gold_ref_ptr(t *data = nullptr) : data(data)
	{
		if (data)
		{
			refcount    = reinterpret_cast<size_t *>(allocator(sizeof(size_t)));
			(*refcount) = 1;
		}
	}

	~gold_ref_ptr()
	{
		if (refcount && !--(*refcount))
		{
			deallocator(refcount);
			if (data)
				deallocator(data);
		}
	}

	template <typename ptrtype> gold_ref_ptr(const gold_ref_ptr<ptrtype> &ptr) : data(ptr.data), refcount(ptr.refcount)
	{
		if (refcount)
			(*refcount)++;
	}

	template <typename ptrtype> gold_ref_ptr &operator=(const gold_ref_ptr<ptrtype> &ptr)
	{
		if (refcount && !--(*refcount))
		{
			deallocator(refcount);
			if (data)
				deallocator(data);
		}

		data     = ptr.data;
		refcount = ptr.refcount;
		if (refcount)
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