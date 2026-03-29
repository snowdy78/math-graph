#pragma once

#include "operator_action.hpp"
#include "func_dependent.hpp"
#include <variant>

namespace mg
{
	std::variant<action, number> get_result(
		const action &act, const var_dependent::map_type &values = {}, const func_dependent::map_type &func_values = {}
	);
	operator_action::forward_type create_parameter_data(const string_type &value);

} // namespace mg
