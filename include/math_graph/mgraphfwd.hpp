#pragma once
#include <string>
#include <sstream>
#include <unordered_set>
#include <vector>

namespace mg
{
	using char_type	   = char;
	using string_type  = std::basic_string<char_type>;
	using sstream_type = std::basic_stringstream<char_type>;
	using cstring_type = const char_type *;
	class expression;
	struct dependent;
	struct dependency_map;
	using dependency_set	= std::unordered_set<const expression *>;
	using dependency_vector = std::vector<const expression *>;
	class number;
	class variable;
	template<class CharType, class RetT, class... Args>
	class basic_operation;
	class binary_operator_action;
	class unary_operator_action;
	class call_function_action;
	struct declaration;
	template<class T>
	struct is_declaration_child
	{
		constexpr static bool value = !std::is_abstract_v<T> && std::is_base_of_v<declaration, T>;
	};
	template<class T>
		requires is_declaration_child<T>::value
	struct definition;
	struct equation;
	class variable_declaration;
	class function_declaration;
	using var = variable;
} // namespace mg
