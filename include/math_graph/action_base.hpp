#pragma once

#include "dependency_map.hpp"
#include "number.hpp"

namespace mg
{
	class action_base : public dependent
	{
	public:
		using result_type	= number;
		using unique_action = std::unique_ptr<action_base>;

		virtual ~action_base()											 = default;
		virtual size_t priority() const									 = 0;
		virtual unique_action copy() const								 = 0;
		virtual result_type evaluate(const dependency_map &values) const = 0;
	};
} // namespace mg
