#pragma once

#include "independent_variable.hpp"
#include "unexpressed_function.hpp"
#include "operation.hpp"
#include <variant>

namespace mg
{
	class unary_operation
	{


	public:
		using operation_pointer	  = const operation *;
		using operation_reference = const operation &;
		using forward_type		  = std::variant<number, independent_variable, unexpressed_function>;
		enum Type
		{
			Plus,
			Minus
		};

	private:
		operation_pointer parse_operator(const string_type &str);
		forward_type parse_operand(const string_type &str);

	public:
		unary_operation(const string_type &str);
		unary_operation(const Type &op, const forward_type &operand);
		const operation &operation() const;
		const forward_type &operand() const;

	private:
		constexpr static const char *s_pattern
			= R"(^\s*[+-]?\s*((?:\d+.?\d*)|\d*(?:\d*.\d+))|([a-zA-Z0-9_]+\s*\([^()]+\))|([a-zA-Z0-9_]+)\s*$)";
		constexpr static const char *s_operator_pattern = R"(^\s*([-])|([+]))";
		operation_pointer m_op;
		forward_type m_operand;
	};
} // namespace mg
