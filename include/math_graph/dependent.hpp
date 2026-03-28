#pragma once

#include "independent_variable.hpp"

namespace mg
{
	struct dependent
	{
		dependent() {}
		dependent(set_dependencies &&deps)
			: var_dependencies(std::move(deps))
		{}
		dependent(const set_dependencies &deps)
			: var_dependencies(deps)
		{}

		const set_dependencies &deps() const
		{
			return var_dependencies;
		}

	protected:
		set_dependencies var_dependencies;
	};
} // namespace mg
