#pragma once

#include "action_base.hpp"
#include "unexpressed_function.hpp"

namespace mg
{
	class call_function_action : public action_base
	{
	public:
		using expression_map_type = std::unordered_map<independent_variable, value_type>;
		call_function_action(const unexpressed_function &func, const expression_map_type &args)
			: m_func(func)
		{
			if (func.arg_count() != args.size())
			{
				throw std::runtime_error("failed to call function with given arguments");
			}
			pull_deps(func);
			for (auto &[key, value]: args)
			{
				m_args.insert({ key, pull_deps(value) });
			}
		}
		result_type evaluate(const var_map_type &vars, const func_map_type &funcs) const override
		{
			if (!var_dependent::defined_in(vars) || !func_dependent::defined_in(funcs))
			{
				return this;
			}
			std::unordered_map<const value_type *, number> forward;
			for (auto &[key, value]: m_args)
			{
				forward.insert({ &value, 0 });
			}
			for (auto &[value, num]: forward)
			{
				auto res = find_value(value, vars, funcs);
				if (std::holds_alternative<action_type>(res))
				{
					return this;
				}
				num = std::get<number>(res);
			}
			var_map_type backward;
			for (auto &[key, value]: m_args)
			{
				backward.insert({ key, forward.at(&value) });
			}
			return funcs.at(m_func)(backward);
		}
		size_t priority() const override
		{
			return 0;
		}

	private:
		unexpressed_function m_func;
		expression_map_type m_args;
	};
} // namespace mg
