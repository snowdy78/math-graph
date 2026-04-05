#include "math_graph/binary_operator_action.hpp"
#include "math_graph/independent_variable.hpp"

namespace mg
{
	action_base::value_type binary_operator_action::try_create_value(const string_type &str)
	{
		try
		{
			return independent_variable(str);
		}
		catch (std::runtime_error &e)
		{
			try
			{
				return number(str);
			}
			catch (std::runtime_error &e)
			{
				throw std::runtime_error("unable to create value by '" + str + "' for binary operator action");
			}
		}
	}
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
		m_left					 = lit_v != left_vars + 2 ? pull_deps(independent_variable(*lit_v)) : number(*lit_n);
		m_op					 = &binary_operation::get_by_name(match[5].str()[0]);
		m_right					 = rit_v != right_vars + 2 ? pull_deps(independent_variable(*rit_v)) : number(*rit_n);
	}
	binary_operator_action::binary_operator_action(
		const value_type &opleft, binary_operation_reference op, const value_type &opright
	)
		: m_left(pull_deps(opleft)),
		  m_right(pull_deps(opright)),
		  m_op(&op)
	{}
	binary_operator_action::binary_operator_action(const string_type &action_str)
	{
		parse(action_str);
	}
	const binary_operator_action::value_type &binary_operator_action::left() const
	{
		return m_left;
	}
	const binary_operator_action::value_type &binary_operator_action::right() const
	{
		return m_right;
	}
	binary_operator_action::binary_operation_reference binary_operator_action::operation() const
	{
		return *m_op;
	}
	action_base::result_type
	binary_operator_action::evaluate(const var_map_type &vars, const func_map_type &funcs) const
	{
		if (!var_dependent::defined_in(vars) || !func_dependent::defined_in(funcs))
		{
			return this;
		}
		std::unordered_map<const value_type *, number> v = {
			{ &m_left,  0 },
			{ &m_right, 0 }
		};
		for (auto &[value, num]: v)
		{
			auto res = find_value(value, vars, funcs);
			if (std::holds_alternative<action_type>(res))
			{
				return this;
			}
			num = std::get<number>(res);
		}
		return operation()(v.at(&m_left), v.at(&m_right));
	}

	size_t binary_operator_action::priority() const
	{
		return m_op->priority();
	}

} // namespace mg
