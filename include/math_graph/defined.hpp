#pragma once

#include "mgraphfwd.hpp"
#include "definition.hpp"
namespace mg
{
	struct defined
	{
		using result_type													= definition::result_type;
		virtual result_type evaluate(const dependency_values &values) const = 0;
	};
} // namespace mg
