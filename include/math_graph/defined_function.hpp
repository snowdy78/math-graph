#pragma once

#include "defined.hpp"
#include "definition.hpp"
#include "function_declaration.hpp"
#include "number.hpp"

namespace mg
{
	struct defined_function : public defined, public function_declaration
	{
		defined_function(const function_declaration &decl, const definition &def)
			: function_declaration(decl),
			  m_def(def)
		{}
		definition::result_type evaluate(const dependency_values &values) const override
		{
			return m_def.evaluate(values);
		}

	private:
		const definition &m_def;
	};
} // namespace mg
