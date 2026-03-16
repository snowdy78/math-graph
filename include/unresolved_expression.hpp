#pragma once

#include "mgraphfwd.hpp"
#include "parameter.hpp"
#include "operation.hpp"

namespace mg
{
	class unresolved_expression
	{
		struct expression_item
		{
			parameter curr_param;
			operation op;

		public:
		};
		std::vector<expression_item> values;

	public:
		unresolved_expression(const variable &var) {}
		unresolved_expression(const parameter &, const unresolved_expression &unr_exp) {}
	};
} // namespace mg
