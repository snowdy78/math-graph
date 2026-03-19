#pragma once

#include "independent_variable.hpp"
#include "number.hpp"
#include "function.hpp"
#include <variant>
#include <list>

namespace mg
{
	class expression
	{
	public:
		using forward_type = std::variant<number, independent_variable, function>;

	private:
		struct expression_item
		{
			expression_item(const forward_type &value, const operation *op = nullptr)
				: m_value(value),
				  m_op(op)
			{}
			forward_type m_value;
			const operation *m_op = nullptr;
		};

	public:
		expression(set_dependencies &&deps, const forward_type &init_action);

		expression &apply(const operation &op, const forward_type &right);
		expression &apply(const function &f);

		const set_dependencies &deps() const;
		string_type str(const string_type &delimiter = "") const;

	private:
		set_dependencies m_deps;
		std::list<expression_item> m_items;
	};
} // namespace mg
