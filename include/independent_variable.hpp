#pragma once

#include "mgraphfwd.hpp"
#include "number.hpp"
#include <regex>
#include <unordered_map>
#include <unordered_set>

namespace mg
{
	class independent_variable
	{
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

	public:
		independent_variable(const string_type &name)
			: m_name(parse(name))
		{}

		const string_type &name() const
		{
			return m_name;
		}
		void rename(const string_type &name)
		{
			m_name = parse(name);
		}
		bool operator==(const independent_variable &other) const
		{
			return &other == this || m_name == other.m_name;
		}
		bool operator<=>(const independent_variable &other) const
		{
			if (m_name < other.m_name)
				return -1;
			if (m_name > other.m_name)
				return 1;
			return 0;
		}

	private:
		constexpr static const char *s_pattern = "^[a-zA-Z][a-zA-Z0-9]*$";

		string_type m_name;
	};

	struct variable_hasher
	{
		std::size_t operator()(const independent_variable &v) const
		{
			return std::hash<std::string>{}(v.name());
		}
	};
	using set_dependencies = std::unordered_set<independent_variable, variable_hasher>;
	using map_dependencies = std::unordered_map<independent_variable, number, variable_hasher>;
} // namespace mg
