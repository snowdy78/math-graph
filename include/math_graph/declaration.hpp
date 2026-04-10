#pragma once

#include "mgraphfwd.hpp"
#include <memory>

namespace mg
{
	struct declaration
	{
		virtual std::unique_ptr<defined> define(const definition *def) const = 0;
	};
} // namespace mg
