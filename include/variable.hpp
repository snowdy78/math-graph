#pragma once

#include "mgraphfwd.hpp"
#include <regex>
#include <stdexcept>

namespace mg
{
	class variable
	{
		constexpr static const char *s_pattern = "^[a-zA-Z][a-zA-Z0-9]*$";

	protected:
		static string_type parse(const string_type &var_name)
		{
			std::regex rgx(s_pattern);
			std::smatch match;
			if (!std::regex_search(var_name, match, rgx))
			{
				throw std::runtime_error("cannot create variable by '" + var_name + "'");
			}
			return match[0];
		}
		string_type m_name;

	public:
		variable(const string_type &name)
			: m_name(parse(name))
		{}
		void rename(const string_type &name)
		{
			m_name = parse(name);
		}
		const string_type &name() const
		{
			return m_name;
		}
	};
} // namespace mg
