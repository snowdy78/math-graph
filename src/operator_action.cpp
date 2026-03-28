#include "math_graph/independent_variable.hpp"
#include "math_graph/tools.hpp"
#include "math_graph/action.hpp"

namespace mg
{
	void operator_action::parse(const string_type &str)
	{
		std::regex op_rgx(s_operator_action_pattern);
		std::smatch match;
		if (!std::regex_search(str, match, op_rgx))
		{
			throw std::runtime_error("unable to create expression by '" + str + "'");
		}
		string_type words[]
			= { match[2].str().empty() ? match[3] : match[2], match[4], match[6].str().empty() ? match[7] : match[6] };
		m_left = create_parameter_data(words[0]);
		find_and_insert_deps(m_left);
		m_op	= &operation::get_by_name(words[1][0]);
		m_right = create_parameter_data(words[2]);
		find_and_insert_deps(m_right);
	}
	void operator_action::find_and_insert_deps(const std::optional<forward_type> &operand)
	{
		if (!operand.has_value())
		{
			throw std::runtime_error("unable to update dependencies of action by given null");
		}
		if (std::holds_alternative<variable_type>(*operand))
		{
			dependencies.insert(std::get<variable_type>(*operand));
		}
		else if (std::holds_alternative<pointer_type>(*operand))
		{
			auto &other_action = std::get<pointer_type>(*operand);
			if (!other_action)
			{
				throw std::runtime_error("unable to update dependencies of action by null pointer");
			}
			auto &other_deps = other_action->deps();
			dependencies.insert(other_deps.begin(), other_deps.end());
		}
	}
	bool operator_action::has_nullptr(const forward_type &operand) const
	{
		return std::holds_alternative<pointer_type>(operand) && !std::get<pointer_type>(operand);
	}
	operator_action::operator_action(const forward_type &opleft, const operation &op, const forward_type &opright)
		: m_left(opleft),
		  m_right(opright),
		  m_op(&op)
	{
		if (has_nullptr(opleft) || has_nullptr(opright))
		{
			throw std::runtime_error("unable to create action by null pointer");
		}
		find_and_insert_deps(opleft);
		find_and_insert_deps(opright);
	}
	operator_action::operator_action(const string_type &action_str)
	{
		parse(action_str);
	}
	const operator_action::forward_type &operator_action::left() const
	{
		return *m_left;
	}
	const operator_action::forward_type &operator_action::right() const
	{
		return *m_right;
	}
	const operation &operator_action::op() const
	{
		return *m_op;
	}
} // namespace mg
