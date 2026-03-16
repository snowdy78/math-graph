#pragma once

#include "mgraphfwd.hpp"
#include <regex>
#include <stdexcept>

namespace mg
{
	class operation
	{
		constexpr static const char *s_pattern = R"(^[\+]|^[\-]|^[\*]|^[\/]|^[\^])";
		static string_type parse(const string_type &op)
		{
			std::regex rgx(s_pattern);
			std::smatch match;
			if (!std::regex_search(op, match, rgx))
			{
				throw std::runtime_error("the name '" + op + "'" + " is not operation");
			}
			return match[0];
		}
		string_type m_op;

	public:
		operation(string_type op)
			: m_op(parse(op))
		{}
	};
} // namespace mg
