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
		using forward_type = std::variant<number, independent_variable, const operator_action *>;

	private:
		void parse(const string_type &str);

	public:
		operator_action(const forward_type &opleft, const operation &op, const forward_type &opright)
			: m_left(opleft),
			  m_right(opright),
			  m_op(&op)
		{}
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
		constexpr static const char *s_operator_action_pattern = R"((.+)(\s+)(.)(\s+)(.+))";

		set_dependencies m_deps;
		std::optional<forward_type> m_left, m_right;
		const operation *m_op = nullptr;
	};
} // namespace mg
