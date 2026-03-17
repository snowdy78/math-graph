#include "operation.hpp"
#include <cmath>
namespace mg
{
	operation::compute_map_type operation::get_compute_map()
	{
		using arg_type		 = const number &;
		compute_map_type map = {
			{ &unique_operations::add,
			  [](arg_type a, arg_type b) {
				  return a + b;
			  } },
			{ &unique_operations::sub,
			  [](arg_type a, arg_type b) {
				  return a - b;
			  } },
			{ &unique_operations::mul,
			  [](arg_type a, arg_type b) {
				  return a * b;
			  } },
			{ &unique_operations::div,
			  [](arg_type a, arg_type b) {
				  return a / b;
			  } },
			{ &unique_operations::pow,
			  [](arg_type a, arg_type b) {
				  return std::pow(a, b);
			  } },
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
