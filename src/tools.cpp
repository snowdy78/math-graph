#include "math_graph/tools.hpp"
#include "math_graph/action.hpp"
#include "math_graph/operation.hpp"

namespace mg
{
	std::variant<action, number> get_result(const action &act, const map_dependencies &values)
	{
		auto deps = act.deps();
		if (deps.size() > values.size())
		{
			return act;
		}
		if (deps.size() == values.size())
		{
			if (act.is_function())
			{
				return act.as_function()(values);
			}
			// operator action otherwise
			auto &opact									 = act.as_operation();
			std::vector<number> nums					 = { 0.0, 0.0 };
			std::vector<operator_action::forward_type> v = { opact.left(), opact.right() };
			for (size_t i = 0; i < nums.size(); ++i)
			{
				auto &value = v[i];
				if (std::holds_alternative<operator_action::variable_type>(value))
				{
					nums[i] = values.at(std::get<operator_action::variable_type>(value));
					continue;
				}
				if (std::holds_alternative<operator_action::number_type>(value))
				{
					nums[i] = std::get<operator_action::number_type>(value);
					continue;
				}
				// value is inner action that way
				// and start recursion by this action
				auto res = get_result({ *std::get<operator_action::pointer_type>(value) }, values);
				if (std::holds_alternative<action>(res))
				{
					return act;
				}
				nums[i] = std::get<number>(res);
			}
			return operation::get_compute_map().at(&opact.op())(nums);
		}
		throw std::runtime_error("too much arguments for action resolving");
	}
	operator_action::forward_type create_parameter_data(const string_type &value)
	{
		try
		{
			return independent_variable(value);
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
} // namespace mg
