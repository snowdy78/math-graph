#pragma once

#include "action_base.hpp"
#include "variable_declaration.hpp"
#include "expression.hpp"
#include "equation.hpp"

namespace mg
{

	struct unpack_variable_action : public action_base
	{
		unpack_variable_action(const string_type &str)
			: m_var(str)
		{
			pull_deps({ { m_var.fullname() } });
		}
		unpack_variable_action(const variable_declaration &var)
			: m_var(var)
		{
			pull_deps({ { m_var.fullname() } });
		}
		size_t priority() const override
		{
			return 0;
		}
		const variable_declaration &variable() const
		{
			return m_var;
		}
		result_type evaluate(const dependency_map &values) const override
		{
			if (!values.contains(m_var.fullname()))
				throw equation({ { m_var.fullname() } });
			return values.at(m_var.fullname())->evaluate(values);
		}
		unique_action copy() const override
		{
			return std::make_unique<unpack_variable_action>(*this);
		}

	private:
		variable_declaration m_var;
	};
} // namespace mg
