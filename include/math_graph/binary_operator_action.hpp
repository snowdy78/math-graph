#pragma once

#include "operation.hpp"
#include "expression.hpp"

namespace mg
{
	class binary_operator_action : public action_base
	{
		void parse(const string_type &str);

	public:
		using operand_type				 = expression;
		using binary_operation_pointer	 = const binary_operation *;
		using binary_operation_reference = const binary_operation &;
		binary_operator_action(const operand_type &opleft, binary_operation_reference op, const operand_type &opright);
		binary_operator_action(const string_type &action_str);
		const operand_type &left() const;
		const operand_type &right() const;
		binary_operation_reference operation() const;
		result_type evaluate(const dependency_map &funcs) const override;
		unique_action copy() const override;
		size_t priority() const override;

	private:
		inline static string_type s_binary_operator_action_pattern = get_pattern(
			R"(^(?:(?:\((?:(${__unary}?(?:\d+\.?\d*|\d*\.\d+))|([a-zA-Z0-9_]+))\))|(?:(${__unary}?(?:\d+\.?\d*|\d*\.\d+))|([a-zA-Z0-9_]+)))\s*(${__binary})\s*(?:(?:\((?:(${__unary}?(?:\d+\.?\d*|\d*\.\d+))|([a-zA-Z0-9_]+))\))|(?:((?:\d+\.?\d*|\d*\.\d+))|([a-zA-Z0-9_]+)))$)",
			{
				{ "${__unary}",	unary_operation::open_pattern()	},
				{ "${__binary}", binary_operation::open_pattern() }
		}
		);

		operand_type m_left, m_right;
		binary_operation_pointer m_op = nullptr;
	};
} // namespace mg
