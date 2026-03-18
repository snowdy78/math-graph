#pragma once

#include "mgraphfwd.hpp"

namespace mg
{
	inline string_type to_pure_string(double num)
	{
		auto str = std::to_string(num);
		while (str.back() == '0')
		{
			str.pop_back();
			if (str.back() == '.')
			{
				str.pop_back();
				break;
			}
		}
		return str;
	}
} // namespace mg
