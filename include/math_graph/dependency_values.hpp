#pragma once

#include "dependent.hpp"

namespace mg
{
	struct dependency_values : dependent::map_type
	{
		using key_type	 = dependent::key_type;
		using value_type = dependent::value_type;
		using set_type	 = dependent::set_type;
		using map_type	 = dependent::map_type;
		using map_type::map_type;
		dependency_values() = default;
		dependency_values(const set_type &set)
		{
			for (const auto &index: set)
			{
				m_map[index] = nullptr;
			}
		}
		void push_deps(const map_type &deps)
		{
			m_map.insert(deps.begin(), deps.end());
		}
		const map_type &get_map() const
		{
			return m_map;
		}
		bool is_initialized(key_type index) const
		{
			return m_map.contains(index) && m_map.at(index) != nullptr;
		}
		bool is_initialized() const
		{
			return !m_map.empty() && !std::all_of(m_map.begin(), m_map.end(), [this](const auto &arg) {
				return is_initialized(arg.first);
			});
		}
		bool is_all_initialized(const set_type &set)
		{
			return std::all_of(set.begin(), set.end(), [this](const auto &arg) {
				return is_initialized(arg);
			});
		}

	private:
		map_type m_map;
	};
} // namespace mg
