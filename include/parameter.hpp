#pragma once

#include "mgraphfwd.hpp"
#include "unnamed_parameter.hpp"
#include <stdexcept>
#include <variant>

namespace mg
{
	class parameter : public variable, public unnamed_parameter
	{
	public:
		explicit parameter(const string_type &name, const string_type &value)
			: variable(name),
			  unnamed_parameter(value)
		{}
		explicit parameter(const string_type &name, const num_or_var_t &value)
			: variable(name),
			  unnamed_parameter(value)
		{}
	};
} // namespace mg
