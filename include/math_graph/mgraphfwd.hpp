#pragma once
#include <string>
#include <sstream>

namespace mg
{
	using char_type	   = char;
	using string_type  = std::basic_string<char_type>;
	using sstream_type = std::basic_stringstream<char_type>;
	using cstring_type = const char_type *;
	class number;
	class independent_variable;
	class dependent;
	class variable;
	class operation;
	class unexpressed_function;
	class function;
	class operator_action;
	class action;
	class expression;

	using var = variable;

} // namespace mg
