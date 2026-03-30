#pragma once

#include "dependent.hpp"
#include "binary_operator_action.hpp"
#include "unary_operator_action.hpp"
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
		using unary_op_type		 = unary_operator_action;
		using binary_op_type	 = binary_operator_action;
		using action_type		 = std::variant<binary_op_type, function_type, unary_op_type>;
		using forward_value_type = std::variant<number, independent_variable>;
		using set_type			 = var_dependent::set_type;
		using map_type			 = var_dependent::map_type;
		action(const action_type &act)
			: m_action(act)
		{}
		bool is_operator_action() const
		{
			return std::holds_alternative<binary_op_type>(m_action);
		}
		bool is_function() const
		{
			return std::holds_alternative<function_type>(m_action);
		}
		bool is_unary_operation() const
		{
			return std::holds_alternative<unary_op_type>(m_action);
		}
		const unary_op_type &as_unary_operation() const
		{
			return std::get<unary_op_type>(m_action);
		}
		const binary_op_type &as_binary_operation() const
		{
			return std::get<binary_op_type>(m_action);
		}
		const function_type &as_function() const
		{
			return std::get<function_type>(m_action);
		}
		const set_type &deps() const
		{
			if (is_function())
			{
				return as_function().deps();
			}
			if (is_operator_action())
			{
				return as_binary_operation().deps();
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
