#pragma once

#include "mgraphfwd.hpp"
#include "variable_declaration.hpp"
#include <list>

namespace mg
{
	class function_declaration : public variable_declaration
	{
	public:
		using return_type = number;
		using args_list	  = std::list<variable_declaration>;

		function_declaration(const string_type &decl_str);
		function_declaration(const string_type &name, args_list &&args);
		function_declaration(const function_declaration &other);
		bool operator==(const function_declaration &other) const;
		bool operator<=>(const function_declaration &other) const;
		function_declaration &operator=(const function_declaration &other);
		std::unique_ptr<defined> define(const definition *def) const override;
		const args_list &args() const;

	private:
		void parse(const string_type &func_str);

	private:
		constexpr static const char *s_syntax_pattern
			= R"(^\s*([a-zA-Z0-9_]+)\s*\((\s*[a-zA-Z0-9_]+(?:\s*,\s*[a-zA-Z0-9_]+)*\s*)\)\s*$)";
		constexpr static const char *s_search_arg_pattern = R"([a-zA-Z0-9_]+)";
		args_list m_args;
	};
} // namespace mg
