#pragma once

#include "dependent.hpp"

namespace mg
{
	struct equation : public dependent
	{
		equation() = default;
		explicit equation(const dependent &deps)
			: dependent(deps)
		{}
		void join(const equation &eq)
		{
			pull_deps(eq.deps());
		}
	};
} // namespace mg
