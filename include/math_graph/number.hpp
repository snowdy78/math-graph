#pragma once

#include "mgraphfwd.hpp"
#include <regex>
#include <stdexcept>
#include "definition.hpp"

namespace mg
{
	class number
	{
		constexpr static const char *s_pattern = R"(^([-+]?)(\d+(\.\d*)?|\.\d+)$)";
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
		number &operator+=(const number &other)
		{
			m_value += other.m_value;
			return *this;
		}
		number &operator-=(const number &other)
		{
			m_value -= other.m_value;
			return *this;
		}
		number &operator*=(const number &other)
		{
			m_value *= other.m_value;
			return *this;
		}
		number &operator/=(const number &other)
		{
			m_value /= other.m_value;
			return *this;
		}
	};
	struct number_definition : public virtual definition
	{
		number_definition(const number &value)
			: m_value(value)
		{}
		std::unique_ptr<definition> copy() const override
		{
			return std::make_unique<number_definition>(m_value);
		}

	private:
		number m_value;
	};
} // namespace mg
