#pragma once

#include "mgraphfwd.hpp"
#include "unnamed_parameter.hpp"
#include "operation.hpp"
#include <optional>

namespace mg
{
	class unresolved_expression
	{
		struct expression_item
		{
			expression_item(const unnamed_parameter &param, const operation *op = nullptr)
				: curr_param(param),
				  op(op)
			{}

			unnamed_parameter curr_param;
			const operation *op = nullptr;
		};
		std::vector<expression_item> m_exprs{};

	public:
		unresolved_expression(const variable &var)
		{
			m_exprs.emplace_back(unnamed_parameter(var));
		}
		unresolved_expression(const unnamed_parameter &, const unresolved_expression &unr_exp) {}

		unresolved_expression &push(const operation &prev_do_for, const unnamed_parameter &param)
		{
			m_exprs.back().op = &prev_do_for;
			m_exprs.push_back(param);
			return *this;
		}
	};
} // namespace mg
