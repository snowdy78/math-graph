#pragma once

#include "mgraphfwd.hpp"
#include "operation.hpp"
#include "unnamed_parameter.hpp"
#include <array>
#include <optional>

namespace mg
{
	class operator_action
	{
		constexpr static const char *s_operator_action_pattern = "(.+)( +)(.+)( +)(.+)";
		constexpr static const char *s_function_action_pattern = "";
		void parse(const string_type &str)
		{
			std::regex op_rgx(s_operator_action_pattern);
			if (!std::regex_match(str, op_rgx))
			{
				throw std::runtime_error("'" + str + "' is not operation");
			}
			sstream_type ss(str);
			std::array<string_type, 3> words;
			string_type word;
			for (size_t i = 0; i < words.size(); i++, ss >> word)
			{
				words[i] = word;
			}
			m_left	= words[0];
			m_op	= operation(words[1]);
			m_right = words[2];
		}
		std::optional<unnamed_parameter> m_left, m_right;
		std::optional<operation> m_op;

	public:
		operator_action(const unnamed_parameter &opleft, const operation &op, const unnamed_parameter &opright)
			: m_left(opleft),
			  m_right(opright),
			  m_op(op)
		{}
		operator_action(const string_type &action_str)
		{
			parse(action_str);
		}
		const unnamed_parameter &left() const
		{
			return *m_left;
		}
		const unnamed_parameter &right() const
		{
			return *m_right;
		}
		const operation &op() const
		{
			return *m_op;
		}
	};
} // namespace mg
