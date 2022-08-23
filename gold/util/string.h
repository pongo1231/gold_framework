#pragma once

#include "gold/memory.h"
#include "gold/util/assert.h"
#include "gold/util/macros.h"

template <void *(*allocator)(size_t) = gold_global_allocate, void (*deallocator)(void *) = gold_global_deallocate>
class gold_string
{
	char *buffer         = nullptr;
	size_t buffer_length = 0;

  public:
	const static inline size_t npos = std::numeric_limits<size_t>::max();

	gold_string()
	{
		clear();
	}

	gold_string(const gold_string &str)
	{
		resize(str.buffer_length);
		memcpy(buffer, str.buffer, str.buffer_length);
	}

	gold_string &operator=(const gold_string &str)
	{
		clear();
		resize(str.buffer_length);
		memcpy(buffer, str.buffer, str.buffer_length);

		return *this;
	}

	gold_string(const gold_string &&str) : buffer(str.buffer), buffer_length(str.buffer_length)
	{
	}

	gold_string &operator=(const gold_string &&str)
	{
		deallocator(buffer);
		buffer        = str.buffer;
		buffer_length = str.buffer_length;

		return *this;
	}

	gold_string(const char *str)
	{
		if (!str)
			clear();
		else
		{
			resize(strlen(str));
			strcpy(buffer, str);
		}
	}

	gold_string &operator=(const char *str)
	{
		clear();

		if (str)
		{
			resize(strlen(str));
			strcpy(buffer, str);
		}

		return *this;
	}

	bool operator==(const gold_string &str) const
	{
		if (!buffer && !str.buffer)
			return true;
		if (!buffer || !str.buffer)
			return false;

		return !strcmp(buffer, str.buffer);
	}

	bool operator==(const char *str) const
	{
		if (!buffer && !str)
			return true;
		if (!buffer || !str)
			return false;

		return !strcmp(buffer, str);
	}

	char *c_string() const
	{
		return buffer;
	}

	size_t length() const
	{
		return strlen(buffer);
	}

	size_t allocated_size() const
	{
		return buffer_length;
	}

	void resize(size_t size)
	{
		auto new_buffer = reinterpret_cast<char *>(allocator(size + 1));
		if (!new_buffer)
			gold_assert("gold_string::resize failed to allocate memory");

		memset(new_buffer, 0, size + 1);

		if (buffer)
		{
			memcpy(new_buffer, buffer, std::min(buffer_length, size));
			deallocator(buffer);
		}

		buffer        = new_buffer;
		buffer_length = size;
	}

	void clear()
	{
		resize(0);
	}

	bool is_empty() const
	{
		return !buffer || !buffer_length;
	}

	gold_string substring(size_t index, size_t count = npos)
	{
		if (index >= buffer_length)
			gold_assert("gold_string::substring index > buffer_length!");

		if (!count)
			return {};

		size_t size = std::min(count, buffer_length - index);

		gold_string str;
		str.resize(size);
		memcpy(str.buffer, buffer + index, size);

		return str;
	}

	size_t find(char character, size_t offset = 0) const
	{
		if (offset >= buffer_length)
			return npos;

		for (char *c = buffer + offset; c < buffer + buffer_length; c++)
		{
			if (*c == character)
				return reinterpret_cast<size_t>(c) - reinterpret_cast<size_t>(buffer);
		}

		return npos;
	}

	size_t find_last(char character, size_t offset = 0) const
	{
		if (offset >= buffer_length)
			return npos;

		size_t i = npos;
		for (char *c = buffer + offset; c < buffer + buffer_length; c++)
		{
			if (*c == character)
				i = reinterpret_cast<size_t>(c) - reinterpret_cast<size_t>(buffer);
		}

		return i;
	}

	size_t find_first_not_of(char character, size_t offset = 0) const
	{
		if (offset >= buffer_length)
			return npos;

		for (char *c = buffer + offset; c < buffer + buffer_length; c++)
		{
			if (*c != character)
				return reinterpret_cast<size_t>(c) - reinterpret_cast<size_t>(buffer);
		}

		return npos;
	}

	size_t find_last_not_of(char character, size_t offset = 0) const
	{
		if (offset >= buffer_length)
			return npos;

		size_t i = npos;
		for (char *c = buffer + offset; c < buffer + buffer_length; c++)
		{
			if (*c != character)
				i = reinterpret_cast<size_t>(c) - reinterpret_cast<size_t>(buffer);
		}

		return i;
	}

	gold_string trim() const
	{
		if (is_empty())
			return {};

		auto first_index = find_first_not_of(' ');
		if (first_index == npos)
			return {};

		gold_string str = buffer;
		str             = str.substring(first_index);
		str             = str.substring(0, find_last_not_of(' ') + 1);

		return str;
	}

	gold_vector<gold_string> split(char delimiter) const
	{
		if (is_empty())
			return {};

		gold_vector<gold_string> parts;
		size_t index_start = 0, index_head = 0;
		while ((index_start = find_first_not_of(delimiter, index_head)) != npos)
		{
			index_head = find(delimiter, index_start);
			parts.push(substring(index_start, index_head == npos ? npos : index_head - index_start));
		}

		return parts;
	}
};