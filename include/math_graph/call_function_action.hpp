#pragma once

#include "action_base.hpp"
#include "function_declaration.hpp"
#include "definition.hpp"
#include "expression.hpp"

namespace mg
{
	class call_function_action : public action_base
	{
		using computed_args = std::vector<expression>;

	public:
		call_function_action(const definition<function_declaration> &func, const dependency_vector &forward_args)
			: m_func(func)
		{
			if (func.args().size() != forward_args.size())
			{
				throw std::runtime_error("failed to call function with given arguments");
			}
			pull_deps(m_func);
			m_args.assign(forward_args.begin(), forward_args.end());
		}
		result_type evaluate(const dependency_map &values) const override
		{
			auto evaluation = evaluate_arguments(values);
			auto arg_values = make_dependency_values(evaluation);
			return m_func.evaluate(arg_values);
		}
		size_t priority() const override
		{
			return 0;
		}
		unique_action copy() const override
		{
			return std::make_unique<call_function_action>(*this);
		}

	private:
		computed_args evaluate_arguments(const dependency_map &values) const
		{
			computed_args eval_args;
			for (auto &arg: m_args)
			{
				eval_args.push_back(arg->evaluate(values));
			}
			return eval_args;
		}
		dependency_map make_dependency_values(const computed_args &exprs) const
		{
			dependency_map arg_values;
			for (size_t i = 0; i < m_args.size(); ++i)
			{
				arg_values.emplace(m_func.args()[i].fullname(), &exprs[i]);
			}
			return arg_values;
		}

	private:
		definition<function_declaration> m_func;
		dependency_vector m_args;
	};
} // namespace mg
