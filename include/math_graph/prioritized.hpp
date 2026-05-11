#pragma once

namespace mg
{
	struct prioritized
	{
		virtual size_t priority() const = 0;
	};
} // namespace mg
