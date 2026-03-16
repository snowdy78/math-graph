#pragma once
#include <string>

namespace mg
{
	using char_type	   = char;
	using string_type  = std::basic_string<char_type>;
	using sstream_type = std::basic_stringstream<char_type>;
	using cstring_type = const char_type *;
	class number;
	class variable;
	class parameter;
	class operation;
	class function;
	class operator_action;
	class action;
	class unresolved_expression;
	class expression;
	class string_expression;

	using var = variable;
	using prm = parameter;
} // namespace mg
