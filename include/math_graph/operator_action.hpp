#pragma once

#include "operation.hpp"
#include "dependent.hpp"
#include <optional>
#include <variant>

namespace mg
{
	class operator_action : public var_dependent
	{
	public:
		using number_type	= number;
		using variable_type = independent_variable;
		using pointer_type	= const action *;
		using forward_type	= std::variant<number_type, variable_type, pointer_type>;

	private:
		void parse(const string_type &str);
		// returns true, if operand has pointer to this action
		bool has_nullptr(const forward_type &operand) const;
		void find_and_insert_deps(const std::optional<forward_type> &operand);

	public:
		operator_action(const forward_type &opleft, const operation &op, const forward_type &opright);
		operator_action(const string_type &action_str);
		const forward_type &left() const;
		const forward_type &right() const;
		const operation &op() const;

	private:
		constexpr static const char *s_operator_action_pattern
			= R"(^(\(([+-]?[a-zA-Z0-9_.]+)\)|([+-]?[a-zA-Z0-9_.]+))\s*(\S)\s*(\(([+-]?[a-zA-Z0-9_.]+)\)|([a-zA-Z0-9_.]+))$)";

		std::optional<forward_type> m_left, m_right;
		const operation *m_op = nullptr;
	};
} // namespace mg
