#pragma once

#include "mgraphfwd.hpp"
#include "parameter.hpp"
#include "operation.hpp"
#include <regex>

namespace mg
{
	class action
	{
		parameter m_left, m_right;
		operation m_op;

	public:
		action(const parameter &opleft, const operation &op, const parameter &opright);
		template<class... Args>
		action(const function &f, const Args &...args);
	};
} // namespace mg
