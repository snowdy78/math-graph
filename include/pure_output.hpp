#pragma once

#include "mgraphfwd.hpp"

namespace mg
{
	inline string_type to_pure_string(double num, size_t precision = 0)
	{
		auto str = std::to_string(num);
		if (str.find('e') != std::string::npos)
			return str;
		size_t pos = str.find('.');
		if (pos == std::string::npos)
			return str;
		size_t dot_distance = str.size() - pos - 1;
		while (str.back() == '0')
		{
			str.pop_back();
			if (--dot_distance == precision)
			{
				break;
			}
		}
		if (str.back() == '.')
			str.pop_back();
		return str;
	}
} // namespace mg
