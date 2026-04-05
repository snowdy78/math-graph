#pragma once

#include <variant>
#include "operation.hpp"
#include "action_base.hpp"

namespace mg
{
	class unary_operator_action : public action_base
	{
	public:
		using operation_pointer	  = const unary_operation *;
		using operation_reference = const unary_operation &;

	private:
		operation_pointer parse_operator(const string_type &str)
		{
			std::regex rgx(s_operator_pattern);
			std::smatch match;
			if (!std::regex_search(str, match, rgx))
			{
				throw std::runtime_error(
					"unable to create unary operation by '" + str + "' (unable to parse operator)"
				);
			}
			if (!match[1].str().empty()) // first group is minus
			{
				return &unique_operations::minus;
			}
			return &unique_operations::plus;
		}
		value_type parse_operand(const string_type &str)
		{
			std::regex rgx(s_pattern);
			std::smatch match;
			if (!std::regex_search(str, match, rgx))
			{
				throw std::runtime_error("unable to unary operation by '" + str + "' (unable to parse operand)");
			}
			if (!match[1].str().empty()) // number
			{
				return number(match[1].str());
			}
			return pull_deps(independent_variable(match[2].str()));
		}

	public:
		unary_operator_action(const string_type &str)
			: m_op(parse_operator(str)),
			  m_operand(parse_operand(str))
		{}
		unary_operator_action(const unary_operation &op, const value_type &operand)
			: m_op(&op),
			  m_operand(pull_deps(operand))
		{}
		operation_reference operation() const
		{
			return *m_op;
		}
		const value_type &operand() const
		{
			return m_operand;
		}
		result_type evaluate(const var_map_type &vars, const func_map_type &funcs) const override
		{
			if (!var_dependent::defined_in(vars) || !func_dependent::defined_in(funcs))
			{
				return this;
			}
			auto res = find_value(&m_operand, vars, funcs);
			if (std::holds_alternative<action_type>(res))
				return this;
			return operation()(std::get<number>(res));
		}
		size_t priority() const override
		{
			return m_op->priority();
		}

	private:
		constexpr static const char *s_pattern = R"(^\s*[+-]?\s*((?:\d+.?\d*)|\d*(?:\d*.\d+))|([a-zA-Z0-9_]+)\s*$)";
		constexpr static const char *s_operator_pattern = R"(^\s*([-])|([+]?))";
		operation_pointer m_op;
		value_type m_operand;
	};
} // namespace mg
