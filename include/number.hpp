#pragma once

#include "mgraphfwd.hpp"
#include <regex>
#include <stdexcept>

namespace mg
{
	class number
	{
		constexpr static const char *s_pattern = R"([0-9]+[\.]?[0-9]*)";
		double m_value;

	private:
		static double parse(const string_type &num)
		{
			std::regex rgx(s_pattern);
			std::smatch match;
			if (!std::regex_search(num, match, rgx))
			{
				throw std::runtime_error("'" + num + "' is not a number");
			}
			return std::stod(match[0]);
		}

	public:
		number(const string_type &num_str)
			: m_value(parse(num_str))
		{}
		number(double n)
			: m_value(n)
		{}

		operator double() const
		{
			return m_value;
		}
	};
} // namespace mg
