#pragma once

#include "defined.hpp"
#include "dependency_values.hpp"
#include "variable_declaration.hpp"
#include "number.hpp"

namespace mg
{
	class defined_variable : public defined, public variable_declaration
	{
	public:
		defined_variable(const variable_declaration &decl, const definition &def)
			: variable_declaration(decl),
			  m_def(&def)
		{
			// recursion exception
			if (deps().contains(fullname()))
			{
				throw std::runtime_error("variable '" + fullname() + "' is recursive assigned");
			}
		}
		result_type evaluate(const dependency_values &values) const override
		{

			return;
		}

	private:
		const definition *const m_def;
	};
} // namespace mg
