#pragma once

#include "action_base.hpp"
#include "dependency_values.hpp"
#include "function_declaration.hpp"

namespace mg
{
	class call_function_action : public action_base
	{
	public:
		using expression_map_type = std::unordered_map<variable_declaration, value_type>;
		call_function_action(const function_declaration &func, const expression_map_type &args)
			: m_func(func)
		{
			if (func.arg_count() != args.size())
			{
				throw std::runtime_error("failed to call function with given arguments");
			}
			pull_deps(&func);
			m_args.insert(args.begin(), args.end());
		}
		result_type evaluate(const dependency_values &values) const override
		{
			return funcs.at(m_func.fullname())(backward);
		}
		size_t priority() const override
		{
			return 0;
		}

	private:
		defined_function m_func;
		expression_map_type m_args;
	};
} // namespace mg
