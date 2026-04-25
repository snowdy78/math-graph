#pragma once

#include "expression.hpp"

namespace mg
{
	template<class TyDecl>
		requires is_declaration_child<TyDecl>::value
	struct definition : TyDecl
	{
		using declaration_type = TyDecl;
		definition(const declaration_type &decl, const expression &def)
			: declaration_type(decl),
			  m_expr(def)
		{}
		const expression &value() const
		{
			return m_expr;
		}
		expression::result_type evaluate(const dependency_map &values) const
		{
			return m_expr.evaluate(values);
		}

	private:
		expression m_expr;
	};
} // namespace mg
