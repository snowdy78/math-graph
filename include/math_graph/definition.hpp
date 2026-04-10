#pragma once

#include "mgraphfwd.hpp"
#include "dependent.hpp"
#include <variant>

namespace mg
{
	struct definition : dependent
	{
		using result_type								 = std::variant<const definition *, number>;
		definition()									 = default;
		virtual std::unique_ptr<definition> copy() const = 0;
	};
} // namespace mg
