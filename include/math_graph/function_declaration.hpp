#pragma once

#include "mgraphfwd.hpp"
#include "variable_declaration.hpp"
#include "dependent.hpp"

namespace mg
{
	class function_declaration : public variable_declaration, public dependent
	{
	public:
		using return_type = number;
		using args_list	  = std::vector<variable_declaration>;
		using args_set	  = dependent::set_type;

		function_declaration(const string_type &decl_str)
			: variable_declaration("f")
		{
			parse(decl_str);
		}
		function_declaration(const string_type &name, args_list &&args)
			: variable_declaration(name),
			  m_args(std::move(args))
		{
			for (auto &arg: m_args)
			{
				if (!dependencies.insert(arg.fullname()).second)
					throw std::runtime_error("duplicate argument name");
			}
		}
		bool operator<=>(const function_declaration &other) const
		{
			if (fullname() > other.fullname())
				return 1;
			if (fullname() < other.fullname())
				return -1;
			return 0;
		}
		const args_list &args() const
		{
			return m_args;
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
			rename(match[1].str());
			string_type params_str = match[2];
			// match data will be 0: full match, 1: func name, 2: param_list inside brackets
			std::regex args_rgx(s_search_arg_pattern);
			std::sregex_iterator it(params_str.begin(), params_str.end(), args_rgx);
			std::sregex_iterator reg_end;
			std::unordered_set<string_type> args;
			for (; it != reg_end; ++it)
			{
				if (args.contains(it->str()))
				{
					throw std::runtime_error("Duplicate argument '" + it->str() + "'");
				}
				if (variable_declaration(it->str()) == *this)
				{
					throw std::runtime_error("Function name '" + fullname() + "' is an argument");
				}
				args.insert(it->str());
				m_args.emplace_back(it->str());
			}
		}

	private:
		constexpr static const char *s_syntax_pattern
			= R"(^\s*([a-zA-Z0-9_]+)\s*\((\s*[a-zA-Z0-9_]+(?:\s*,\s*[a-zA-Z0-9_]+)*\s*)\)\s*$)";
		constexpr static const char *s_search_arg_pattern = R"([a-zA-Z0-9_]+)";
		args_list m_args;
	};
} // namespace mg
