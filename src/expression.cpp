#include "expression.hpp"
#include "action.hpp"

namespace mg
{
	expression::expression(set_dependencies &&deps, const forward_type &init_value)
	{
		m_deps = std::move(deps);
		m_items.emplace_back(init_value);
	}
	expression &expression::apply(const operation &to_prev_action, const forward_type &value)
	{
		if (std::holds_alternative<independent_variable>(value))
		{
			m_deps.insert(std::get<independent_variable>(value));
		}
		m_items.emplace_back(value, &to_prev_action);
		return *this;
	}
	expression &expression::apply(const function &f)
	{
		auto args = f.args();
		m_deps.merge(args);
		m_items.emplace_back(f);
		return *this;
	}
	const set_dependencies &expression::deps() const
	{
		return m_deps;
	}
} // namespace mg
