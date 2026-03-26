#pragma once

#include "mgraphfwd.hpp"
#include "independent_variable.hpp"
#include "unexpressed_function.hpp"
#include <functional>

namespace mg
{
	class function : public unexpressed_function
	{
	public:
		using return_type	= number;
		using params_type	= map_dependencies;
		using function_type = std::function<return_type(const params_type &)>;

		function(const string_type &decl_str, const function_type &f)
			: unexpressed_function(decl_str),
			  m_func(f)
		{}
		function(const string_type &name, set_dependencies &&args, const function_type &f)
			: unexpressed_function(name, std::move(args)),
			  m_func(f)
		{}
		function(unexpressed_function &&other, const function_type &f)
			: unexpressed_function(std::move(other)),
			  m_func(f)
		{}
		function(const unexpressed_function &other, const function_type &f)
			: unexpressed_function(other),
			  m_func(f)
		{}
		return_type operator()(const map_dependencies &_args) const
		{
			if (std::any_of(_args.begin(), _args.end(), [&](const auto &arg) {
					return !args().contains(arg.first);
				}))
			{
				throw std::runtime_error("unknown argument in function '" + fullname() + "'");
			}
			if (_args.size() > args().size())
			{
				throw std::runtime_error("too many arguments in function '" + fullname() + "'");
			}
			return m_func(_args);
		}
		const function_type &func() const
		{
			return m_func;
		}
		bool operator==(const function &other) const
		{
			return unexpressed_function::operator==(other);
		}
		bool operator<=>(const function &other) const
		{
			return unexpressed_function::operator<=>(other);
		}

	private:
		function_type m_func;
	};
} // namespace mg
