#pragma once

#include "mgraphfwd.hpp"
#include "variable.hpp"
#include "number.hpp"
#include <variant>

namespace mg
{
	class unnamed_parameter
	{
	protected:
		using num_or_var_t = std::variant<number, variable>;

	private:
		num_or_var_t m_value;
		num_or_var_t parse_parameter(const string_type &value)
		{
			try
			{
				return variable(value);
			}
			catch (std::runtime_error &)
			{
				try
				{
					return number(value);
				}
				catch (std::runtime_error &err)
				{
					throw std::runtime_error("can't create parameter by '" + value + "'");
				}
			}
		}

	public:
		unnamed_parameter(const string_type &value)
			: m_value(parse_parameter(value))
		{}
		unnamed_parameter(const num_or_var_t &value)
			: m_value(value)
		{}
		bool is_variable() const
		{
			return std::holds_alternative<variable>(m_value);
		}
		bool is_number() const
		{
			return std::holds_alternative<number>(m_value);
		}
		const number &as_number() const
		{
			return std::get<number>(m_value);
		}
		const variable &as_variable() const
		{
			return std::get<variable>(m_value);
		}
	};
} // namespace mg
