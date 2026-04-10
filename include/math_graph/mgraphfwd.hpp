#pragma once
#include <string>
#include <sstream>

namespace mg
{
	using char_type	   = char;
	using string_type  = std::basic_string<char_type>;
	using sstream_type = std::basic_stringstream<char_type>;
	using cstring_type = const char_type *;
	struct dependent;
	struct dependency_values;
	class number;
	class variable_declaration;
	class variable;
	template<class CharType, class RetT, class... Args>
	class basic_operation;
	class function_declaration;
	class function;
	class action_base;
	class binary_operator_action;
	class unary_operator_action;
	class call_function_action;
	class action;
	class expression;
	struct definition;
	struct declaration;
	struct defined;
	class variable_declaration;
	class variable_definition;
	class function_declaration;
	class function_definition;
	using var = variable;
} // namespace mg
