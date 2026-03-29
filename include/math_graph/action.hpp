#pragma once

#include "dependent.hpp"
#include "operator_action.hpp"
#include "unary_operation.hpp"
#include "unexpressed_function.hpp"
#include <variant>

namespace mg
{
	class action
	{
	public:
		template<class T>
		using reference = T &;
		template<class T>
		using const_reference	 = const T &;
		using function_type		 = unexpressed_function;
		using action_type		 = std::variant<operator_action, function_type, unary_operation>;
		using forward_value_type = std::variant<number, independent_variable>;
		using set_type			 = var_dependent::set_type;
		using map_type			 = var_dependent::map_type;
		action(const action_type &act)
			: m_action(act)
		{}
		bool is_operator_action() const
		{
			return std::holds_alternative<operator_action>(m_action);
		}
		bool is_function() const
		{
			return std::holds_alternative<function_type>(m_action);
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
		const function_type &as_function() const
		{
			return std::get<function_type>(m_action);
		}
		const set_type &deps() const
		{
			if (is_function())
			{
				return std::get<function_type>(m_action).deps();
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
