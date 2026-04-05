#pragma once

#include "operation.hpp"
#include "action_base.hpp"

namespace mg
{
	class binary_operator_action : public action_base
	{
		// throws if can't create
		value_type try_create_value(const string_type &str);
		void parse(const string_type &str);

	public:
		using binary_operation_pointer	 = const binary_operation *;
		using binary_operation_reference = const binary_operation &;
		binary_operator_action(const value_type &opleft, binary_operation_reference op, const value_type &opright);
		binary_operator_action(const string_type &action_str);
		const value_type &left() const;
		const value_type &right() const;
		binary_operation_reference operation() const;
		result_type evaluate(const var_map_type &vars, const func_map_type &funcs) const override;

		size_t priority() const override;

	private:
		constexpr static const char *s_binary_operator_action_pattern
			= R"(^(?:(?:\((?:([+-]?(?:\d+\.?\d*|\d*\.\d+))|([a-zA-Z0-9_]+))\))|(?:([+-]?(?:\d+\.?\d*|\d*\.\d+))|([a-zA-Z0-9_]+)))\s*([+\-\*\/\^])\s*(?:(?:\((?:([+-]?(?:\d+\.?\d*|\d*\.\d+))|([a-zA-Z0-9_]+))\))|(?:((?:\d+\.?\d*|\d*\.\d+))|([a-zA-Z0-9_]+)))$)";

		value_type m_left, m_right;
		binary_operation_pointer m_op = nullptr;
	};
} // namespace mg
