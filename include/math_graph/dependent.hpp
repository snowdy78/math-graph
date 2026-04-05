#pragma once

#include "independent_variable.hpp"
#include <unordered_map>
#include <unordered_set>

namespace mg
{
	template<class Kty, class Vty, class Hasher = std::hash<Kty>>
	struct dependent
	{
		using key_type	  = Kty;
		using value_type  = Vty;
		using hasher_type = Hasher;
		using set_type	  = std::unordered_set<key_type, hasher_type>;
		using map_type	  = std::unordered_map<key_type, value_type, hasher_type>;

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
		set_type dependencies;
	};

	struct variable_hasher
	{
		std::size_t operator()(const independent_variable &v) const
		{
			return std::hash<std::string>{}(v.fullname());
		}
	};
	struct var_dependent : dependent<independent_variable, number, variable_hasher>
	{
		using dependent<independent_variable, number, variable_hasher>::dependent;
	};
} // namespace mg
