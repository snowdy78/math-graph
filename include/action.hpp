#pragma once

#include "mgraphfwd.hpp"
#include "operator_action.hpp"
#include "function.hpp"
#include <variant>

namespace mg
{
	class action
	{
		std::variant<operator_action, function> m_action;

	public:
		action(const operator_action &op_act)
			: m_action(op_act)
		{}
		action(const function &f)
			: m_action(f)
		{}
	};
} // namespace mg
