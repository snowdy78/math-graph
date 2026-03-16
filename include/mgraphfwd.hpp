#pragma once
#include <string>

namespace mg
{
	using char_type	  = char;
	using string_type = std::basic_string<char_type>;

	class parameter;
	class variable;
	class unresolved_expression;
	class operand;
	class function;
	class action;
	class expression;
	class string_expression;
} // namespace mg
