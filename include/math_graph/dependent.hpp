#pragma once

#include "independent_variable.hpp"

namespace mg
{
	struct dependent
	{
		dependent() {}
		dependent(set_dependencies &&deps)
			: dependencies(std::move(deps))
		{}
		dependent(const set_dependencies &deps)
			: dependencies(deps)
		{}

		const set_dependencies &deps() const
		{
			return dependencies;
		}

	protected:
		set_dependencies dependencies;
	};
} // namespace mg
