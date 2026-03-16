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
	class unresolved_expression;
	class operation;
	class function;
	class action;
	class expression;
	class string_expression;

	using var = variable;
	using prm = parameter;
} // namespace mg
