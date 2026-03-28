#pragma once

#include "unexpressed_function.hpp"

namespace mg
{
	struct unexpressed_function_hasher : std::hash<std::string>
	{
		std::size_t operator()(const unexpressed_function &f) const
		{
			return std::hash<std::string>{}(f.fullname());
		}
	};
	struct func_dependent
		: dependent<unexpressed_function, unexpressed_function::function_type, unexpressed_function_hasher>
	{
		using dependent<
			unexpressed_function, unexpressed_function::function_type, unexpressed_function_hasher>::dependent;
	};
} // namespace mg
