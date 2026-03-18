#pragma once

#include "mgraphfwd.hpp"
#include "pure_output.hpp"
#include "unnamed_parameter.hpp"
#include "operation.hpp"

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
			const operation *op;
		};
		std::vector<expression_item> m_exprs{};

	public:
		unresolved_expression(const variable &var)
		{
			m_exprs.emplace_back(unnamed_parameter(var));
		}
		unresolved_expression(unresolved_expression &&unr_exp, const unnamed_parameter &param)
		{
			m_exprs = std::move(unr_exp.m_exprs);
			m_exprs.emplace_back(param);
		}

		unresolved_expression &push(const operation &prev_do_for, const unnamed_parameter &param)
		{
			m_exprs.back().op = &prev_do_for;
			m_exprs.push_back(param);
			return *this;
		}
		string_type str(const string_type &delimiter = "") const
		{
			string_type result;
			for (auto i = m_exprs.begin(); i != m_exprs.end(); ++i)
			{
				if (i->curr_param.is_number())
				{
					result += to_pure_string(i->curr_param.as_number());
				}
				else
				{
					result += i->curr_param.as_variable().name();
				}
				if (i->op)
				{
					result += delimiter;
					result += i->op->get();
					result += delimiter;
				}
			}
			return result;
		}
	};
} // namespace mg
