#pragma once
#include <string>
#include <sstream>

namespace mg
{
	using char_type	   = char;
	using string_type  = std::basic_string<char_type>;
	using sstream_type = std::basic_stringstream<char_type>;
	using cstring_type = const char_type *;
	template<class Kty, class Vty, class Hasher>
	class dependent;
	class number;
	class independent_variable;
	class var_dependent;
	class variable;
	class operation;
	class unexpressed_function;
	class func_dependent;
	class function;
	class operator_action;
	class action;
	class expression;

	using var = variable;

} // namespace mg
