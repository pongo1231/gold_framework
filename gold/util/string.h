#pragma once

#include "gold/util/vector.h"

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
	text = text.substr(0, text.find_last_of(' '));
}

inline gold_vector<std::string> gold_string_split(std::string_view delimiter)
{
}