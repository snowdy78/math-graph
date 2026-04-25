#pragma once

#include "operation.hpp"
#include "action_base.hpp"
#include "unpack_variable_action.hpp"

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
		expression parse_operand(const string_type &str)
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
			auto var_name = match[2].str();
			pull_deps({ { var_name } });
			return unpack_variable_action(var_name);
		}

	public:
		unary_operator_action(const string_type &str)
			: m_op(parse_operator(str)),
			  m_operand(parse_operand(str))
		{}
		unary_operator_action(const unary_operation &op, const expression &operand)
			: m_op(&op),
			  m_operand(operand)
		{}
		operation_reference operation() const
		{
			return *m_op;
		}
		const expression &operand() const
		{
			return m_operand;
		}
		result_type evaluate(const dependency_map &funcs) const override
		{
			return m_operand.evaluate(funcs);
		}
		size_t priority() const override
		{
			return m_op->priority();
		}

		unique_action copy() const override
		{
			return std::make_unique<unary_operator_action>(*this);
		}

	private:
		constexpr static const char *s_pattern = R"(^\s*[+-]?\s*((?:\d+.?\d*)|\d*(?:\d*.\d+))|([a-zA-Z0-9_]+)\s*$)";
		constexpr static const char *s_operator_pattern = R"(^\s*([-])|([+]?))";
		operation_pointer m_op;
		expression m_operand;
	};
} // namespace mg
