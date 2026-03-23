#pragma once

#include "mgraphfwd.hpp"
#include "independent_variable.hpp"
#include <initializer_list>
#include <regex>
#include <functional>

namespace mg
{
	class function
	{
	public:
		using return_type	= number;
		using params_type	= map_dependencies;
		using function_type = std::function<return_type(const params_type &)>;

		function(const string_type &decl_str, const function_type &f)
			: m_func(f)
		{
			parse(decl_str);
		}
		function(const string_type &name, set_dependencies &&args, const function_type &f)
			: m_name(name),
			  m_args(std::move(args)),
			  m_func(f)
		{}
		const string_type &name() const
		{
			return m_name;
		}
		size_t arg_count() const
		{
			return m_args.size();
		}
		const set_dependencies &args() const
		{
			return m_args;
		}
		return_type operator()(const map_dependencies &args) const
		{
			if (std::any_of(args.begin(), args.end(), [&](const auto &arg) {
					return !m_args.contains(arg.first);
				}))
			{
				throw std::runtime_error("unknown argument in function '" + m_name + "'");
			}
			if (args.size() > m_args.size())
			{
				throw std::runtime_error("too many arguments in function '" + m_name + "'");
			}
			return m_func(args);
		}

	private:
		void parse(const string_type &func_str)
		{
			std::regex rgx(s_syntax_pattern);
			std::smatch match;
			if (!std::regex_search(func_str, match, rgx))
			{
				throw std::runtime_error("Invalid Function Syntax: Unable to create a function by '" + func_str + "'");
			}
			m_name				   = match[1];
			string_type params_str = match[2];
			// match data will be 0: full match, 1: func name, 2: param_list inside brackets
			std::regex args_rgx(s_search_arg_pattern);
			std::sregex_iterator it(params_str.begin(), params_str.end(), args_rgx);
			std::sregex_iterator reg_end;
			for (; it != reg_end; ++it)
			{
				if (m_args.contains({ it->str() }))
				{
					throw std::runtime_error("Duplicate argument '" + it->str() + "' in function '" + m_name + "'");
				}
				m_args.insert({ it->str() });
			}
		}

	private:
		constexpr static const char *s_syntax_pattern
			= R"(^\s*([a-zA-Z]+\d*)\s*\((\s*[a-zA-Z]+\d*(?:\s*,\s*[a-zA-Z]+\d*)*\s*)\)\s*$)";
		constexpr static const char *s_search_arg_pattern = R"([a-zA-Z]+\d*)";

		string_type m_name;
		set_dependencies m_args; // function arguments
		function_type m_func;
	};
} // namespace mg
