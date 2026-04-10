#pragma once

#include "mgraphfwd.hpp"
#include "definition.hpp"

namespace mg
{
	class action_base : public virtual definition
	{
	public:
		virtual ~action_base()			= default;
		virtual size_t priority() const = 0;
	};
} // namespace mg
