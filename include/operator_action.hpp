#pragma once

#include "mgraphfwd.hpp"
#include "operation.hpp"
#include "unresolved_expression.hpp"
#include <array>
#include <optional>

namespace mg
{
	class operator_action
	{
	public:
		using result_type = std::variant<unresolved_expression, number>;

	private:
		constexpr static const char *s_operator_action_pattern = R"((.+)(\s+)(.)(\s+)(.+))";
		constexpr static const char *s_function_action_pattern = "";
		void parse(const string_type &str)
		{
			std::regex op_rgx(s_operator_action_pattern);
			std::smatch match;
			if (!std::regex_search(str, match, op_rgx))
			{
				throw std::runtime_error("unable to create expression by '" + str + "'");
			}
			std::array<string_type, 3> words = { match[1], match[3], match[5] };
			m_left							 = words[0];
			m_op							 = &operation::get_by_name(words[1][0]);
			m_right							 = words[2];
		}
		std::optional<unnamed_parameter> m_left, m_right;
		const operation *m_op = nullptr;

	public:
		operator_action(const unnamed_parameter &opleft, const operation &op, const unnamed_parameter &opright)
			: m_left(opleft),
			  m_right(opright),
			  m_op(&op)
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
		result_type compute() const
		{
			if (m_left->is_variable() || m_right->is_variable())
			{
				auto &guarantee_var = m_left->is_variable() ? *m_left : *m_right;
				auto &probably_var	= m_left->is_variable() ? *m_right : *m_left;
				return unresolved_expression(guarantee_var.as_variable()).push(*m_op, probably_var);
			}
			return operation::get_compute_map().at(&*m_op)(m_left->as_number(), m_right->as_number());
		}
	};
} // namespace mg
