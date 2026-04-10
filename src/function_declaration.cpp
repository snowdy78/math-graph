#include "math_graph/function_declaration.hpp"
#include <unordered_set>
#include "math_graph/variable_declaration.hpp"
#include "math_graph/defined_function.hpp"

namespace mg
{
	function_declaration::function_declaration(const string_type &decl_str)
		: variable_declaration("f")
	{
		parse(decl_str);
	}
	function_declaration::function_declaration(const string_type &name, args_list &&args)
		: variable_declaration(name),
		  m_args(std::move(args))
	{}
	bool function_declaration::operator==(const function_declaration &other) const
	{
		return &other == this || fullname() == other.fullname();
	}
	bool function_declaration::operator<=>(const function_declaration &other) const
	{
		if (fullname() > other.fullname())
			return 1;
		if (fullname() < other.fullname())
			return -1;
		return 0;
	}
	std::unique_ptr<defined> function_declaration::define(const definition *def) const
	{
		return std::make_unique<defined_function>(*this, *def);
	}
	const function_declaration::args_list &function_declaration::args() const
	{
		return m_args;
	}

	void function_declaration::parse(const string_type &func_str)
	{
		std::regex rgx(s_syntax_pattern);
		std::smatch match;
		if (!std::regex_search(func_str, match, rgx))
		{
			throw std::runtime_error("Invalid Function Syntax: Unable to create a function by '" + func_str + "'");
		}
		rename(match[1].str());
		string_type params_str = match[2];
		// match data will be 0: full match, 1: func name, 2: param_list inside brackets
		std::regex args_rgx(s_search_arg_pattern);
		std::sregex_iterator it(params_str.begin(), params_str.end(), args_rgx);
		std::sregex_iterator reg_end;
		std::unordered_set<variable_declaration> args;
		for (; it != reg_end; ++it)
		{
			if (args.contains({ it->str() }))
			{
				throw std::runtime_error("Duplicate argument '" + it->str() + "'");
			}
			if (args.contains(fullname()))
			{
				throw std::runtime_error("Function name '" + fullname() + "' is an argument");
			}
			args.insert({ it->str() });
		}
		m_args.assign(m_args.begin(), m_args.end());
	}
} // namespace mg
