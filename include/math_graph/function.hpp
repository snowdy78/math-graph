#pragma once

#include "mgraphfwd.hpp"
#include "function_declaration.hpp"
#include "number.hpp"
#include <functional>

namespace mg
{
	class function : public function_declaration
	{
	public:
		using return_type	= number;
		using function_type = std::function<return_type(const map_type &)>;

		function(const string_type &decl_str, const function_type &f)
			: function_declaration(decl_str),
			  m_func(f)
		{}
		function(const string_type &name, set_type &&args, const function_type &f)
			: function_declaration(name, std::move(args)),
			  m_func(f)
		{}
		function(function_declaration &&other, const function_type &f)
			: function_declaration(std::move(other)),
			  m_func(f)
		{}
		function(const function_declaration &other, const function_type &f)
			: function_declaration(other),
			  m_func(f)
		{}
		return_type operator()(const map_type &args) const
		{
			if (!defined_in(args))
			{
				throw std::runtime_error("failed to call function with given arguments");
			}
			return m_func(args);
		}
		const function_type &func() const
		{
			return m_func;
		}
		bool operator==(const function &other) const
		{
			return function_declaration::operator==(other);
		}
		bool operator<=>(const function &other) const
		{
			return function_declaration::operator<=>(other);
		}

	private:
		function_type m_func;
	};
} // namespace mg
