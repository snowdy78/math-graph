#pragma once

#include "mgraphfwd.hpp"
#include "unnamed_parameter.hpp"

namespace mg
{

	class function
	{
		constexpr static const char *s_pattern					= R"(^([a-zA-Z0-9]*)\((.*)\)$)";
		constexpr static const char *s_param_separation_pattern = R"(\s*,\s*)";

		void parse(const string_type &func_str)
		{
			std::regex rgx(s_pattern);
			std::smatch match;
			if (!std::regex_search(func_str, match, rgx))
			{
				throw std::runtime_error("unable to create a function by '" + func_str + "'");
			}
			// match data will be 0: full match, 1: func name, 2: param_list inside brackets
			m_name = match[1];
			std::regex sep_rgx(s_param_separation_pattern);
			string_type params_str = match[2];
			std::sregex_token_iterator it(params_str.begin(), params_str.end(), sep_rgx, -1);
			std::sregex_token_iterator reg_end;

			for (; it != reg_end; ++it)
			{
				m_params.emplace_back(it->str());
			}
		}
		string_type m_name;
		std::vector<unnamed_parameter> m_params;

	public:
		function(const string_type &str);
		function(const string_type &name, const std::initializer_list<unnamed_parameter> &params)
			: m_name(name),
			  m_params(params.begin(), params.end())
		{}
	};
} // namespace mg
