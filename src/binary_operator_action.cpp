#include "math_graph/binary_operator_action.hpp"
#include "math_graph/dependency_map.hpp"
#include "math_graph/unpack_variable_action.hpp"
#include "math_graph/number.hpp"
#include "math_graph/equation.hpp"
#include <optional>

namespace mg
{
	void binary_operator_action::parse(const string_type &str)
	{
		std::regex op_rgx(s_binary_operator_action_pattern);
		std::smatch match;
		if (!std::regex_search(str, match, op_rgx))
		{
			throw std::runtime_error("unable to create expression by '" + str + "'");
		}
		string_type left_nums[]	 = { match[1], match[3] };
		string_type left_vars[]	 = { match[2], match[4] };
		string_type right_nums[] = { match[6], match[8] };
		string_type right_vars[] = { match[7], match[9] };
		auto lit_v				 = std::find_if(left_vars, left_vars + 2, [](const string_type &s) {
			  return !s.empty();
		  });
		auto rit_v				 = std::find_if(right_vars, right_vars + 2, [](const string_type &s) {
			  return !s.empty();
		  });
		m_op					 = &binary_operation::get_by_name(match[5].str()[0]);

		if (lit_v != left_vars + 2)
		{
			m_left = unpack_variable_action{ *lit_v };
			pull_deps(m_left);
		}
		else
		{
			auto lit_n = std::find_if(left_nums, left_nums + 2, [](const string_type &s) {
				return !s.empty();
			});
			m_left	   = number(*lit_n);
		}
		if (rit_v != right_vars + 2)
		{
			m_right = unpack_variable_action(*rit_v);
			pull_deps(m_right);
		}
		else
		{
			auto rit_n = std::find_if(right_nums, right_nums + 2, [](const string_type &s) {
				return !s.empty();
			});
			m_right	   = number(*rit_n);
		}
	}
	binary_operator_action::binary_operator_action(
		const operand_type &opleft, binary_operation_reference op, const operand_type &opright
	)
		: m_left(opleft),
		  m_right(opright),
		  m_op(&op)
	{
		pull_deps(m_left.deps());
		pull_deps(m_right.deps());
	}
	binary_operator_action::binary_operator_action(const string_type &action_str)
		: m_left(number(0)),
		  m_right(number(0))
	{
		parse(action_str);
	}
	const binary_operator_action::operand_type &binary_operator_action::left() const
	{
		return m_left;
	}
	const binary_operator_action::operand_type &binary_operator_action::right() const
	{
		return m_right;
	}
	binary_operator_action::binary_operation_reference binary_operator_action::operation() const
	{
		return *m_op;
	}
	action_base::result_type binary_operator_action::evaluate(const dependency_map &values) const
	{
		std::optional<equation> left_eq{}, right_eq{};
		std::optional<result_type> left, right;
		try
		{
			left = m_left.evaluate(values);
		}
		catch (const equation &eq)
		{
			left_eq = eq;
		}
		try
		{
			right = m_right.evaluate(values);
		}
		catch (const equation &eq)
		{
			right_eq = eq;
		}
		if (left_eq || right_eq)
		{
			left_eq->join(*right_eq);
			if (left_eq && right_eq)
			{
				left_eq->join(*right_eq);
				throw *left_eq;
			}
			throw left_eq.has_value() ? *left_eq : *right_eq;
		}
		return operation()(*left, *right);
	}
	action_base::unique_action binary_operator_action::copy() const
	{
		return std::make_unique<binary_operator_action>(*this);
	}

	size_t binary_operator_action::priority() const
	{
		return m_op->priority();
	}

} // namespace mg
