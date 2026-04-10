#include "math_graph/binary_operator_action.hpp"
#include <variant>
#include "math_graph/dependency_values.hpp"
#include "math_graph/variable_definition.hpp"

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
		auto lit_n				 = std::find_if(left_nums, left_nums + 2, [](const string_type &s) {
			  return !s.empty();
		  });
		auto lit_v				 = std::find_if(left_vars, left_vars + 2, [](const string_type &s) {
			  return !s.empty();
		  });
		auto rit_n				 = std::find_if(right_nums, right_nums + 2, [](const string_type &s) {
			  return !s.empty();
		  });
		auto rit_v				 = std::find_if(right_vars, right_vars + 2, [](const string_type &s) {
			  return !s.empty();
		  });
		m_left					 = lit_v != left_vars + 2 ? std::move(variable_definition(*lit_v).copy())
														  : std::move(number_definition(*lit_n).copy());
		m_op					 = &binary_operation::get_by_name(match[5].str()[0]);
		m_right					 = rit_v != right_vars + 2 ? std::move(variable_definition(*rit_v).copy())
														   : std::move(number_definition(*rit_n).copy());
	}
	binary_operator_action::binary_operator_action(
		const value_type &opleft, binary_operation_reference op, const value_type &opright
	)
		: m_left(std::move(opleft->copy())),
		  m_right(std::move(opright->copy())),
		  m_op(&op)
	{
		pull_deps(*m_left);
		pull_deps(*m_right);
	}
	binary_operator_action::binary_operator_action(const string_type &action_str)
	{
		parse(action_str);
		pull_deps(*m_left);
		pull_deps(*m_right);
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
	action_base::result_type binary_operator_action::evaluate(const dependency_values &values) const
	{
		auto left = m_left->evaluate(values);
		if (auto left_value = std::get_if<number>(&left))
		{
			auto right = m_right->evaluate(values);
			if (auto right_value = std::get_if<number>(&right))
			{
				return operation()(*left_value, *right_value);
			}
		}
		return this;
	}

	size_t binary_operator_action::priority() const
	{
		return m_op->priority();
	}

} // namespace mg
