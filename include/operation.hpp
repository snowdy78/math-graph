#pragma once

#include "mgraphfwd.hpp"
#include "unnamed_parameter.hpp"
#include <regex>
#include <stdexcept>
#include <variant>
namespace mg
{

	class operation
	{
		using result_type = std::variant<number, unresolved_expression>;

		constexpr static const char *s_pattern = R"(^[\+]|^[\-]|^[\*]|^[\/]|^[\^])";

	private:
		string_type parse(const string_type &op)
		{
			std::regex rgx(s_pattern);
			std::smatch match;
			if (!std::regex_search(op, match, rgx))
			{
				throw std::runtime_error("the name '" + op + "'" + " is not operation");
			}
			m_name = match[0];
			return m_name;
		}
		result_type add(const unnamed_parameter &left, const unnamed_parameter &right)
		{
			if (left.is_number() && right.is_number())
			{
				return left.as_number() + right.as_number();
			}
		}

	public:
		operation(const string_type &op)
			: m_name(parse(op))
		{}

		string_type get() const
		{
			return m_name;
		}
		result_type compute() const
		{
			if (m_name == "+")
			{}
		}

	private:
		string_type m_name;
	};
} // namespace mg
