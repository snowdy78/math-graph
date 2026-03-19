#pragma once

#include "independent_variable.hpp"
#include "operator_action.hpp"
#include <variant>

namespace mg
{
	std::variant<action, number> get_result(const action &act, const map_dependencies &values);
	operator_action::forward_type create_parameter_data(const string_type &value);

} // namespace mg
