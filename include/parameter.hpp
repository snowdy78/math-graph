#pragma once

#include "mgraphfwd.hpp"
#include "unnamed_parameter.hpp"

namespace mg
{
	class parameter : public variable, public unnamed_parameter
	{
	public:
		explicit parameter(const string_type &name, const unnamed_parameter &value)
			: variable(name),
			  unnamed_parameter(value)
		{}
	};
} // namespace mg
