#pragma once

#include "independent_variable.hpp"
#include "operator_action.hpp"
#include "unary_operation.hpp"
#include "function.hpp"
#include <variant>

namespace mg
{
	class action
	{
	public:
		using action_type		 = std::variant<operator_action, function, unary_operation>;
		using forward_value_type = std::variant<number, independent_variable>;
		action(const action_type &act)
			: m_action(act)
		{}
		bool is_operator_action() const
		{
			return std::holds_alternative<operator_action>(m_action);
		}
		bool is_function() const
		{
			return std::holds_alternative<function>(m_action);
		}
		bool is_unary_operation() const
		{
			return std::holds_alternative<unary_operation>(m_action);
		}
		const unary_operation &as_unary_operation() const
		{
			return std::get<unary_operation>(m_action);
		}
		const operator_action &as_operation() const
		{
			return std::get<operator_action>(m_action);
		}
		const function &as_function() const
		{
			return std::get<function>(m_action);
		}
		const set_dependencies &deps() const
		{
			if (is_function())
			{
				return std::get<function>(m_action).args();
			}
			if (is_operator_action())
			{
				return std::get<operator_action>(m_action).deps();
			}
			if (is_unary_operation())
			{
				return as_unary_operation().deps();
			}
			throw std::runtime_error("unknown action type");
		}

	private:
		action_type m_action;
	};
} // namespace mg
