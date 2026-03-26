#include "math_graph/unary_operation.hpp"
#include "math_graph/unexpressed_function.hpp"

namespace mg
{
	unary_operation::forward_type unary_operation::parse_operand(const string_type &str)
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
		if (!match[2].str().empty()) // function
		{
			return mg::unexpressed_function(match[2].str());
		}
		return independent_variable(match[3].str());
	}
	unary_operation::operation_pointer unary_operation::parse_operator(const string_type &str)
	{
		std::regex rgx(s_operator_pattern);
		std::smatch match;
		if (!std::regex_search(str, match, rgx))
		{
			throw std::runtime_error("unable to create unary operation by '" + str + "' (unable to parse operator)");
		}
		if (!match[1].str().empty()) // first group is minus
		{
			return &unique_operations::minus;
		}
		return &unique_operations::plus;
	}
	unary_operation::unary_operation(const string_type &str)
		: m_op(parse_operator(str)),
		  m_operand(parse_operand(str))
	{}
	unary_operation::unary_operation(const Type &op, const forward_type &operand)
		: m_op(op == Plus ? &unique_operations::plus : &unique_operations::minus),
		  m_operand(operand)
	{}
	const operation &unary_operation::operation() const
	{
		return *m_op;
	}
	const unary_operation::forward_type &unary_operation::operand() const
	{
		return m_operand;
	}
} // namespace mg
