#pragma once

#include "mgraphfwd.hpp"
#include <unordered_map>
#include <unordered_set>
#include <algorithm>


namespace mg
{
	struct dependent
	{
		using key_type	 = string_type;
		using value_type = const expression *;
		using set_type	 = std::unordered_set<key_type>;
		using map_type	 = std::unordered_map<key_type, value_type>;

		dependent() {}
		dependent(set_type &&deps)
			: dependencies(std::move(deps))
		{}
		dependent(const set_type &deps)
			: dependencies(deps)
		{}

		const set_type &deps() const
		{
			return dependencies;
		}
		dependent except(const set_type &set) const
		{
			set_type m;
			for (auto &i: deps())
			{
				if (!set.contains(i))
					m.insert(i);
			}
			return m;
		}
		bool defined_in(const map_type &map) const
		{
			return std::all_of(
					   map.begin(), map.end(),
					   [&](const auto &arg) {
						   return deps().contains(arg.first);
					   }
				   )
				   && map.size() == deps().size();
		}

	protected:
		void pull_deps(const dependent &def)
		{
			auto &deps = def.deps();
			dependencies.insert(deps.begin(), deps.end());
		}
		set_type dependencies;
	};
} // namespace mg
