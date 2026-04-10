#pragma once

#include "mgraphfwd.hpp"
#include "definition.hpp"
#include "dependency_values.hpp"
#include <functional>
#include "number.hpp"

namespace mg
{
	struct function_definition : virtual definition
	{
		using func_type = std::function<number(const dependency_values &)>;
		function_definition(const func_type &func, const dependency_values &args)
			: m_func(func),
			  m_args(args)
		{}

		std::unique_ptr<definition> copy() const override
		{
			return std::make_unique<function_definition>(*this);
		}
		const func_type &func() const
		{
			return m_func;
		}
		const dependency_values &args() const
		{
			return m_args;
		}

	private:
		func_type m_func;
		dependency_values m_args;
	};
} // namespace mg
