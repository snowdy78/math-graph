#pragma once

#include "operation.hpp"
#include "independent_variable.hpp"
#include <optional>
#include <variant>

namespace mg
{
	class operator_action
	{
	public:
		using number_type	= number;
		using variable_type = independent_variable;
		using pointer_type	= const operator_action *;
		using forward_type	= std::variant<number_type, variable_type, pointer_type>;

	private:
		void parse(const string_type &str);
		// returns true, if operand has pointer to this action
		bool has_recursive_reference(const forward_type &operand) const
		{
			return std::holds_alternative<pointer_type>(operand) && std::get<pointer_type>(operand) == this;
		}
		void find_and_insert_deps(const std::optional<forward_type> &operand)
		{
			if (!operand.has_value())
			{
				throw std::runtime_error("unable to update dependencies of action by given null");
			}
			if (std::holds_alternative<variable_type>(*operand))
			{
				m_deps.insert(std::get<variable_type>(*operand));
			}
			else if (std::holds_alternative<pointer_type>(*operand))
			{
				auto &other_action = std::get<pointer_type>(*operand);
				if (!other_action)
				{
					throw std::runtime_error("unable to update dependencies of action by null pointer");
				}
				auto &other_deps = other_action->deps();
				m_deps.insert(other_deps.begin(), other_deps.end());
			}
		}

	public:
		operator_action(const forward_type &opleft, const operation &op, const forward_type &opright)
			: m_left(opleft),
			  m_right(opright),
			  m_op(&op)
		{
			if (has_recursive_reference(opleft) || has_recursive_reference(opright))
			{
				throw std::runtime_error("unable to create operator action with itself reference");
			}
			find_and_insert_deps(opleft);
			find_and_insert_deps(opright);
		}
		operator_action(const string_type &action_str)
		{
			parse(action_str);
		}
		const forward_type &left() const
		{
			return *m_left;
		}
		const set_dependencies &deps() const
		{
			return m_deps;
		}
		const forward_type &right() const
		{
			return *m_right;
		}
		const operation &op() const
		{
			return *m_op;
		}

	private:
		constexpr static const char *s_operator_action_pattern
			= R"(^(\(([+-]?[a-zA-Z0-9.]+)\)|([+-]?[a-zA-Z0-9.]+))\s*(\S)\s*(\(([+-]?[a-zA-Z0-9.]+)\)|([a-zA-Z0-9.]+))$)";

		set_dependencies m_deps;
		std::optional<forward_type> m_left, m_right;
		const operation *m_op = nullptr;
	};
} // namespace mg
