#pragma once

#include "mgraphfwd.hpp"
#include "func_dependent.hpp"
#include "number.hpp"
#include <variant>

namespace mg
{
	class action_base : public var_dependent, public func_dependent
	{
		using func_dependent::deps;
		using var_dependent::deps;

	public:
		using func_map_type = func_dependent::map_type;
		using var_map_type	= var_dependent::map_type;
		using func_set_type = func_dependent::set_type;
		using var_set_type	= var_dependent::set_type;
		using action_type	= const action_base *;
		using number_type	= number;
		using function_type = unexpressed_function;
		using variable_type = independent_variable;
		using value_type	= std::variant<action_type, number_type, function_type, variable_type>;
		using result_type	= std::variant<action_type, number_type>;

	protected:
		const value_type &pull_deps(const value_type &v)
		{
			if (std::holds_alternative<action_type>(v))
			{
				auto act = std::get<action_type>(v);
				if (!act)
				{
					throw std::runtime_error("can't pull dependencies action is nullptr");
				}
				var_dependent::dependencies.insert(act->var_deps().begin(), act->var_deps().end());
				func_dependent::dependencies.insert(act->func_deps().begin(), act->func_deps().end());
			}
			else if (auto *func = std::get_if<function_type>(&v))
			{
				func_dependent::dependencies.insert(*func);
			}
			else if (auto *var = std::get_if<variable_type>(&v))
			{
				var_dependent::dependencies.insert(*var);
			}
			return v;
		}
		result_type find_value(const value_type *value, const var_map_type &vars, const func_map_type &funcs) const
		{
			if (!value)
				throw std::runtime_error("failed to evaluate binary operator action");
			if (auto var = std::get_if<variable_type>(value))
			{
				return vars.at(*var);
			}
			if (auto n = std::get_if<number_type>(value))
			{
				return *n;
			}
			if (auto func = std::get_if<function_type>(value))
			{
				return funcs.at(*func)(vars);
			}
			if (auto action_ptr = std::get_if<action_type>(value))
			{
				auto action = *action_ptr;
				if (!action)
					throw std::runtime_error("failed to evaluate binary operator action");
				result_type res = action->evaluate(vars, funcs);
				if (std::holds_alternative<action_type>(res))
				{
					return this;
				}
				return std::get<number>(res);
			}
			return this;
		}

	public:
		virtual ~action_base() = default;

		const var_set_type &var_deps() const
		{
			return var_dependent::deps();
		}
		const func_set_type &func_deps() const
		{
			return func_dependent::deps();
		}

		virtual result_type evaluate(const var_map_type &values, const func_map_type &definitions) const = 0;
		virtual size_t priority() const																	 = 0;
	};
} // namespace mg
