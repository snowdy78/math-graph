#include "math_graph/operation.hpp"
#include <cmath>
namespace mg
{
	const operation::compute_map_type &operation::get_compute_map()
	{
		using arg_type				= const number &;
		static compute_map_type map = {
			{ &unique_operations::add,
			  [](auto &vs) {
				  return vs.at(0) + vs.at(1);
			  } },
			{ &unique_operations::sub,
			  [](auto &vs) {
				  return vs.at(0) - vs.at(1);
			  } },
			{ &unique_operations::mul,
			  [](auto &vs) {
				  return vs.at(0) * vs.at(1);
			  } },
			{ &unique_operations::div,
			  [](auto &vs) {
				  return vs.at(0) / vs.at(1);
			  } },
			{ &unique_operations::pow,
			  [](auto &vs) {
				  return std::pow(vs.at(0), vs.at(1));
			  } },
			{ &unique_operations::plus,
			  [](auto &vs) {
				  return +vs.at(0);
			  } },
			{ &unique_operations::minus,
			  [](auto &vs) {
				  return -vs.at(0);
			  } }
		};
		return map;
	}

	bool operation::operator==(const operation &other) const
	{
		return &m_name == &other.m_name || m_name == other.m_name;
	}
	const operation &operation::get_by_name(const key_type &key)
	{
		auto op				 = parse(key);
		compute_map_type map = get_compute_map();
		return *std::find_if(map.begin(), map.end(), [&op](const auto &pair) {
					return pair.first->get() == op;
				})->first;
	}
} // namespace mg
