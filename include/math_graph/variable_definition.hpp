#pragma once

#include "mgraphfwd.hpp"
#include "definition.hpp"
#include "variable_declaration.hpp"
#include "number.hpp"

namespace mg
{
	struct variable_definition : public virtual definition
	{
		variable_definition(const variable_declaration &var)
			: m_var(var)
		{}
		std::unique_ptr<definition> copy() const override
		{
			return std::make_unique<variable_definition>(*this);
		}

	private:
		variable_declaration m_var;
	};
} // namespace mg
