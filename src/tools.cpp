#include "tools.hpp"
#include "action.hpp"
#include "operation.hpp"

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
			auto &opact						   = act.as_operation();
			constexpr size_t n				   = 2;
			number nums[n]					   = { 0.0, 0.0 };
			operator_action::forward_type v[n] = { opact.left(), opact.right() };
			for (size_t i = 0; i < n; ++i)
			{
				auto &value = v[i];
				if (std::holds_alternative<independent_variable>(value))
				{
					nums[i] = values.at(std::get<independent_variable>(value));
					continue;
				}
				if (std::holds_alternative<number>(value))
				{
					nums[i] = std::get<number>(value);
					continue;
				}
				// value is inner action that way
				// and start recursion by this action
				auto res = get_result({ *std::get<operator_action::pointer_type>(value) }, values);
				if (std::holds_alternative<action>(res))
				{
					return act;
				}
				else
				{
					nums[i] = std::get<number>(value);
				}
			}
			return operation::get_compute_map()[&opact.op()](nums[0], nums[1]);
		}
		throw std::runtime_error("count of values for action resolving is to much");
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
