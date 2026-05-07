#pragma once

#include "mgraphfwd.hpp"

namespace mg
{
	struct declaration
	{
		virtual ~declaration() = 0;
	};
	inline declaration::~declaration() {}
} // namespace mg
