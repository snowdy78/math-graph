#pragma once

#include "mgraphfwd.hpp"
#include "number.hpp"
#include <regex>
#include <stdexcept>
#include <variant>

namespace mg
{
	class variable
	{
	public:
		using computable_type = number;
		using parametric_type = const variable *;
		using value_type	  = std::variant<computable_type, parametric_type>;

	private:
		constexpr static const char *s_pattern = "^[a-zA-Z][a-zA-Z0-9]*$";

	protected:
		static string_type parse(const string_type &var_name)
		{
			std::regex rgx(s_pattern);
			std::smatch match;
			if (!std::regex_search(var_name, match, rgx))
			{
				throw std::runtime_error("cannot create variable by '" + var_name + "'");
			}
			return match[0];
		}
		string_type m_name;
		value_type m_value = nullptr;
		bool is_computable = false;

	public:
		variable(const string_type &name)
			: m_name(parse(name))
		{}
		// parameter behaviour
		variable(const string_type &name, const variable &other)
			: m_name(other.m_name),
			  m_value(&other),
			  is_computable(other.is_computable)
		{}
		variable(const string_type &name, const number &num)
			: m_name(name),
			  m_value(num),
			  is_computable(true)
		{}
		bool computable() const
		{
			return is_computable;
		}
		bool has_refer() const
		{
			return std::holds_alternative<parametric_type>(m_value) && std::get<parametric_type>(m_value)
				   || std::holds_alternative<computable_type>(m_value);
		}
		bool number_ref() const
		{
			return std::holds_alternative<computable_type>(m_value);
		}
		bool variable_ref() const
		{
			return std::holds_alternative<parametric_type>(m_value);
		}
		bool independent() const
		{
			return std::holds_alternative<parametric_type>(m_value) && !std::get<parametric_type>(m_value);
		}
		const number &as_number() const
		{
			return std::get<computable_type>(m_value);
		}
		const variable &as_variable() const
		{
			return *std::get<parametric_type>(m_value);
		}
		void rename(const string_type &name)
		{
			m_name = parse(name);
		}
		const string_type &name() const
		{
			return m_name;
		}
		friend const variable &find_final_variable(const variable &var);
	};
	inline const variable &find_final_variable(const variable &var)
	{
		if (var.has_refer())
		{
			return find_final_variable(*std::get<variable::parametric_type>(var.m_value));
		}
		return var;
	}
} // namespace mg
