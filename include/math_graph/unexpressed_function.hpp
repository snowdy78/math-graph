#pragma once

#include "mgraphfwd.hpp"
#include "dependent.hpp"
#include <initializer_list>
#include <regex>
#include <functional>

namespace mg
{
	class unexpressed_function : public independent_variable, public dependent
	{
	public:
		using return_type = number;
		using params_type = map_dependencies;

		using function_type = std::function<return_type(const params_type &)>;

		unexpressed_function(const string_type &decl_str)
			: independent_variable("f")
		{
			parse(decl_str);
		}
		unexpressed_function(const string_type &name, set_dependencies &&args)
			: independent_variable(name),
			  dependent(std::move(args))
		{}
		size_t arg_count() const
		{
			return var_dependencies.size();
		}

		bool operator==(const unexpressed_function &other) const
		{
			return &other == this || fullname() == other.fullname();
		}
		bool operator<=>(const unexpressed_function &other) const
		{
			if (fullname() > other.fullname())
				return 1;
			if (fullname() < other.fullname())
				return -1;
			return 0;
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
			for (; it != reg_end; ++it)
			{
				if (var_dependencies.contains({ it->str() }))
				{
					throw std::runtime_error("Duplicate argument '" + it->str() + "'");
				}
				if (var_dependencies.contains(fullname()))
				{
					throw std::runtime_error("Function name '" + fullname() + "' is an argument");
				}
				var_dependencies.insert({ it->str() });
			}
		}

	private:
		constexpr static const char *s_syntax_pattern
			= R"(^\s*([a-zA-Z0-9_]+)\s*\((\s*[a-zA-Z0-9_]+(?:\s*,\s*[a-zA-Z0-9_]+)*\s*)\)\s*$)";
		constexpr static const char *s_search_arg_pattern = R"([a-zA-Z0-9_]+)";
	};
	struct unexpressed_function_hasher : std::hash<std::string>
	{
		std::size_t operator()(const unexpressed_function &f) const
		{
			return std::hash<std::string>{}(f.fullname());
		}
	};
	using set_func_dependencies = std::unordered_set<unexpressed_function, unexpressed_function_hasher>;
	using map_func_dependencies
		= std::unordered_map<unexpressed_function, unexpressed_function::function_type, unexpressed_function_hasher>;
} // namespace mg
