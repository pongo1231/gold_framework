#pragma once

#include "gold/memory.h"

#include <string>
#include <string_view>

inline void gold_string_trim(std::string &text)
{
	if (text.empty())
		return;

	auto first_index = text.find_first_not_of(' ');
	if (first_index == text.npos)
	{
		text.clear();
		return;
	}

	text = text.substr(first_index);
	text = text.substr(0, text.find_last_not_of(' ') + 1);
}

inline gold_vector<std::string> gold_string_split(std::string_view text, const auto &delimiter)
{
	gold_vector<std::string> parts;
	size_t index_start = 0, index_head = 0;
	while ((index_start = text.find_first_not_of(delimiter, index_head)) != text.npos)
	{
		index_head = text.find(delimiter, index_start);
		parts.push(
		    std::string(text.substr(index_start, index_head == text.npos ? text.npos : index_head - index_start)));
	}

	return parts;
}