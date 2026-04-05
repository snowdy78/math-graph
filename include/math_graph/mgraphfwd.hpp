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
	template<class CharType, class RetT, class... Args>
	class basic_operation;
	class unexpressed_function;
	class func_dependent;
	class function;
	class action_base;
	class binary_operator_action;
	class unary_operator_action;
	class call_function_action;
	class action;
	class expression;

	using var = variable;
} // namespace mg
