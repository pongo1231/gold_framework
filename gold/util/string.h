#pragma once

#include "gold/memory.h"
#include "gold/util/assert.h"

#include <limits>
#include <string_view>

/*
* Gold string class
*/
template <void *(*allocator)(size_t), void (*deallocator)(void *)> class gold_base_string
{
	char *buffer         = nullptr;
	size_t buffer_length = 0;

  public:
	/*
	* Placeholder value for invalid values
	*/
	const static inline size_t npos = std::numeric_limits<size_t>::max();

	/*
	* Default constructor with empty string
	*/
	gold_base_string()
	{
		clear();
	}

	/*
	* Copy constructor
	* <param name="str">Other string</param>
	*/
	gold_base_string(const gold_base_string &str)
	{
		resize(str.buffer_length);
		memcpy(buffer, str.buffer, str.buffer_length);
	}

	/*
	 * Copy operator
	 * <param name="str">Other string</param>
	 */
	gold_base_string &operator=(const gold_base_string &str)
	{
		clear();
		resize(str.buffer_length);
		memcpy(buffer, str.buffer, str.buffer_length);

		return *this;
	}

	/*
	 * Move constructor
	 * <param name="str">Other string</param>
	 */
	gold_base_string(const gold_base_string &&str) : buffer(str.buffer), buffer_length(str.buffer_length) noexcept
	{
	}

	/*
	 * Move operator
	 * <param name="str">Other string</param>
	 */
	gold_base_string &operator=(const gold_base_string &&str) noexcept
	{
		if (buffer)
			deallocator(buffer);
		buffer        = str.buffer;
		buffer_length = str.buffer_length;

		return *this;
	}

	/*
	 * Constructor
	 * <param name="str">C String to copy to internal buffer</param>
	 */
	gold_base_string(const char *str)
	{
		if (!str)
			clear();
		else
		{
			resize(strlen(str));
			strcpy(buffer, str);
		}
	}

	gold_base_string &operator=(const char *str)
	{
		clear();

		if (str)
		{
			resize(strlen(str));
			strcpy(buffer, str);
		}

		return *this;
	}

	bool operator==(const gold_base_string &str) const
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

	/*
	 * Convertion from gold_base_string to std::string_view
	 */
	operator std::string_view() const
	{
		return buffer;
	}

	friend std::ostream &operator<<(std::ostream &out, const gold_base_string &str)
	{
		out << str.buffer;
		return out;
	}

	char &operator[](size_t index) const
	{
		return buffer[index];
	}

	/*
	* <returns>Raw string</returns>
	*/
	char *c_string() const
	{
		return buffer;
	}

	/*
	* <returns>Length of string</returns>
	*/
	size_t length() const
	{
		return strlen(buffer);
	}

	/*
	* <returns>Size of current allocation</returns>
	*/
	size_t allocated_size() const
	{
		return buffer_length;
	}

	/*
	* Resize internal buffer, cuts off existing string if the new size is smaller than old size
	* Allocates an additional byte for zero terminator
	* <param name="size">New size of buffer</param>
	*/
	void resize(size_t size)
	{
		auto new_buffer = reinterpret_cast<char *>(allocator(size + 1));
		if (!new_buffer)
			gold_assert("gold_base_string::resize failed to allocate memory");

		memset(new_buffer, 0, size + 1);

		if (buffer)
		{
			memcpy(new_buffer, buffer, std::min(buffer_length, size));
			deallocator(buffer);
		}

		buffer        = new_buffer;
		buffer_length = size;
	}

	/*
	* Resize buffer to 0 bytes
	*/
	void clear()
	{
		resize(0);
	}

	/*
	* Whether there's either no allocation or the length of the buffer is 0 bytes
	*/
	bool is_empty() const
	{
		return !buffer || !buffer_length;
	}

	/*
	* <param name="index">Index to use as start of substring, index above allocated size is invalid</param>
	* <param name="count">Amount of characters to include in substring starting from starting index, defaults to end of the string</param>
	* <returns>A new string constructed from the characters between index and index + count</returns>
	*/
	gold_base_string substring(size_t index, size_t count = npos) const
	{
		if (index >= buffer_length)
			gold_assert("gold_base_string::substring index > buffer_length!");

		if (!count)
			return {};

		size_t size = std::min(count, buffer_length - index);

		gold_base_string str;
		str.resize(size);
		memcpy(str.buffer, buffer + index, size);

		return str;
	}

	/*
	 * <param name="sequence">Sequence of characters to search for</param>
	 * <param name="offset">Starting index to search from</param>
	 * <returns>Starting index of where sequence of characters has first occured, or npos if not found</returns>
	 */
	size_t find(std::string_view sequence, size_t offset = 0) const
	{
		if (sequence.empty() || offset >= buffer_length)
			return npos;

		size_t i = 0;
		for (char *c = buffer + offset; c < buffer + buffer_length; c++)
		{
			if (*c != sequence[i])
				i = 0;
			else if (++i == sequence.length())
				return reinterpret_cast<size_t>(c) - reinterpret_cast<size_t>(buffer);
		}

		return npos;
	}

	/*
	 * <param name="sequence">Sequence of characters to search for</param>
	 * <param name="offset">Starting index to search from</param>
	 * <returns>Starting index of where sequence of characters has last occured, or npos if not found</returns>
	 */
	size_t find_last(std::string_view sequence, size_t offset = 0) const
	{
		if (sequence.empty() || offset >= buffer_length)
			return npos;

		size_t i = 0, last_found = npos;
		for (char *c = buffer + offset; c < buffer + buffer_length; c++)
		{
			if (*c != sequence[i])
				i = 0;
			else if (++i == sequence.length())
			{
				i          = 0;
				last_found = reinterpret_cast<size_t>(c) - reinterpret_cast<size_t>(buffer);
			}
		}

		return last_found;
	}

	/*
	 * <param name="sequence">Sequence of characters to search for</param>
	 * <param name="offset">Starting index to search from</param>
	 * <returns>Starting index of where sequence of characters has first not occured, or npos if not found</returns>
	 */
	size_t find_first_not_of(std::string_view sequence, size_t offset = 0) const
	{
		if (sequence.empty() || offset >= buffer_length)
			return npos;

		size_t i = 0;
		for (char *c = buffer + offset; c < buffer + buffer_length; c++)
		{
			if (*c == sequence[i])
				i = 0;
			else if (++i == sequence.length())
				return reinterpret_cast<size_t>(c) - reinterpret_cast<size_t>(buffer);
		}

		return npos;
	}

	/*
	 * <param name="sequence">Sequence of characters to search for</param>
	 * <param name="offset">Starting index to search from</param>
	 * <returns>Starting index of where sequence of characters has last not occured, or npos if not found</returns>
	 */
	size_t find_last_not_of(std::string_view sequence, size_t offset = 0) const
	{
		if (sequence.empty() || offset >= buffer_length)
			return npos;

		size_t i = 0, found_last = npos;
		for (char *c = buffer + offset; c < buffer + buffer_length; c++)
		{
			if (*c == sequence[i])
				i = 0;
			else if (++i == sequence.length())
			{
				i          = 0;
				found_last = reinterpret_cast<size_t>(c) - reinterpret_cast<size_t>(buffer);
			}
		}

		return found_last;
	}

	/*
	 * <param name="sequence">Sequence of characters to search for</param>
	 * <returns>Whether string contains sequence of characters</returns>
	 */
	bool contains(std::string_view sequence) const
	{
		return find(sequence) != npos;
	}

	/*
	 * <param name="sequence">Sequence of characters to search for</param>
	 * <returns>Amount of times sequence occured in string</returns>
	 */
	size_t count(std::string_view sequence) const
	{
		size_t i = 0, offset = 0;
		while (true)
		{
			offset = find(sequence, offset);
			if (offset == npos)
				break;

			i++;
			offset++;
		}

		return i;
	}

	/*
	 * <returns>New string with whitespace trimmed on left and right side of string</returns>
	 */
	gold_base_string trim() const
	{
		if (is_empty())
			return {};

		auto first_index = find_first_not_of(" ");
		if (first_index == npos)
			return {};

		gold_base_string str = buffer;
		str                  = str.substring(first_index);
		str                  = str.substring(0, find_last_not_of(" ") + 1);

		return str;
	}

	/*
	 * <param name="delimiter">Sequence of characters to split string by</param>
	 * <returns>List of strings splitted by delimiter</returns>
	 */
	gold_vector<gold_base_string> split(std::string_view delimiter) const
	{
		if (is_empty())
			return {};

		gold_vector<gold_base_string> parts;
		size_t index_start = 0, index_head = 0;
		while ((index_start = find_first_not_of(delimiter, index_head)) != npos)
		{
			index_head = find(delimiter, index_start);
			parts.push(substring(index_start, index_head == npos ? npos : index_head - index_start));
		}

		return parts;
	}
};

/*
* String with default gold framework memory allocator and deallocator
*/
using gold_string = gold_base_string<gold_global_allocate, gold_global_deallocate>;