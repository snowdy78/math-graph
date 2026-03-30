#pragma once

#include "dependent.hpp"
#include "operation.hpp"
#include <variant>

namespace mg
{
	class unary_operator_action : public var_dependent
	{
	public:
		using operation_pointer	  = const unary_operation *;
		using operation_reference = const unary_operation &;
		using action_pointer	  = const action *;
		using forward_type		  = std::variant<number, independent_variable, action_pointer>;

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
		forward_type parse_operand(const string_type &str)
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
			auto var = independent_variable(match[2].str());
			dependencies.insert(var);
			return var;
		}

	public:
		unary_operator_action(const string_type &str)
			: m_op(parse_operator(str)),
			  m_operand(parse_operand(str))
		{}
		unary_operator_action(const unary_operation &op, const forward_type &operand)
			: m_op(&op),
			  m_operand(operand)
		{}
		unary_operator_action(const unary_operation &op, forward_type &&operand)
			: m_op(&op),
			  m_operand(std::move(operand))
		{}
		const unary_operation &operation() const
		{
			return *m_op;
		}
		const forward_type &operand() const
		{
			return m_operand;
		}

	private:
		constexpr static const char *s_pattern = R"(^\s*[+-]?\s*((?:\d+.?\d*)|\d*(?:\d*.\d+))|([a-zA-Z0-9_]+)\s*$)";
		constexpr static const char *s_operator_pattern = R"(^\s*([-])|([+]?))";
		operation_pointer m_op;
		forward_type m_operand;
	};
} // namespace mg
