#include "math_graph/variable_declaration.hpp"
#include "math_graph/defined_variable.hpp"

namespace mg
{
	std::unique_ptr<defined> variable_declaration::define(const definition *def) const
	{
		if (auto var_def = dynamic_cast<const variable_definition *>(def))
		{
			return std::make_unique<defined_variable>(*this, *var_def);
		}
		return nullptr;
	}
} // namespace mg
