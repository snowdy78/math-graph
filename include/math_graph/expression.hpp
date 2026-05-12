#pragma once

#include "action_base.hpp"
#include <functional>
#include <variant>
#include "function_declaration.hpp"

namespace mg
{
	using function_dependencies = std::unordered_map<string_type, definition<function_declaration>>;
	struct expression : action_base
	{
	private:
		using function_type = std::function<result_type(const dependency_map &)>;
		using value_type	= std::variant<number, function_type, unique_action>;

		static value_type copy_value(const expression &other)
		{
			return std::visit<value_type>(
				[](const auto &value) {
					using T = std::decay_t<decltype(value)>;
					if constexpr (std::is_same_v<unique_action, T>)
					{
						return value->copy();
					}
					else
					{
						return value;
					}
				},
				other.m_value
			);
		}
		static value_type parse(const string_type &str, const function_dependencies &);

	public:
		expression(const string_type &str, const function_dependencies &deps = {})
			: m_value(parse(str, deps))
		{}
		expression(const number &n)
			: m_value(n)
		{}
		expression(const action_base &other)
			: m_value(other.copy())
		{
			pull_deps(other);
		}
		expression(const function_type &func)
			: m_value(func)
		{}
		expression(unique_action &&ptr_other)
			: m_value(std::move(ptr_other))
		{
			pull_deps(*ptr_other);
		}
		expression(const expression &other)
			: action_base(other),
			  m_value(copy_value(other))
		{}

		bool is_number() const
		{
			return std::holds_alternative<number>(m_value);
		}
		const number &as_number() const
		{
			return std::get<number>(m_value);
		}
		bool is_function() const
		{
			return std::holds_alternative<function_type>(m_value);
		}
		const function_type &as_function() const
		{
			return std::get<function_type>(m_value);
		}
		bool is_expression() const
		{
			return std::holds_alternative<unique_action>(m_value);
		}
		const action_base &as_expression() const
		{
			return *std::get<unique_action>(m_value);
		}
		size_t priority() const override
		{
			if (is_expression())
			{
				return as_expression().priority();
			}
			return 0;
		}
		bool has_solution(const dependency_map &values) const
		{
			return values.all_initialized(deps());
		}
		result_type evaluate(const dependency_map &values = {}) const override
		{
			if (!has_solution(values))
				throw std::runtime_error("expression has no solution");
			if (is_number())
				return as_number();
			if (is_function())
				return as_function()(values);
			return as_expression().evaluate(values);
		}
		unique_action copy() const override
		{
			return std::make_unique<expression>(*this);
		}
		expression &operator=(const expression &other)
		{
			if (this != &other)
			{
				static_cast<action_base &>(*this) = other;
				m_value							  = copy_value(other);
			}
			return *this;
		}

	private:
		value_type m_value;
	};
} // namespace mg
