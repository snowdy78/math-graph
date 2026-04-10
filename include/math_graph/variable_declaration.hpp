#pragma once

#include "mgraphfwd.hpp"
#include <regex>
#include "declaration.hpp"

namespace mg
{
	class variable_declaration : public virtual declaration
	{
		string_type parse(const string_type &var_name)
		{
			std::regex rgx(s_pattern);
			std::smatch match;
			if (!std::regex_search(var_name, match, rgx))
			{
				throw std::runtime_error("cannot create variable by '" + var_name + "'");
			}
			auto &numerator = match[3].str().empty() ? match[4] : match[3];
			auto numstr		= numerator.str();
			return match[1].str() + (numstr.empty() ? "" : "_" + numstr);
		}

	public:
		using defined_type = variable_definition;
		variable_declaration(const string_type &name)
		{
			rename(name);
		}

		string_type name() const
		{
			auto sep = m_name.find('_');
			if (sep == string_type::npos)
				return m_name;
			return m_name.substr(0, sep);
		}
		string_type numerator() const
		{
			auto sep = m_name.find('_');
			if (sep == string_type::npos)
				return "";
			return m_name.substr(sep + 1);
		}
		const string_type &fullname() const
		{
			return m_name;
		}
		void rename(const string_type &name)
		{
			m_name = parse(name);
		}
		bool operator==(const variable_declaration &other) const
		{
			return &other == this || fullname() == other.fullname();
		}
		bool operator<=>(const variable_declaration &other) const
		{
			if (fullname() < other.fullname())
				return -1;
			if (fullname() > other.fullname())
				return 1;
			return 0;
		}
		std::unique_ptr<defined> define(const definition *def) const override;

	private:
		constexpr static const char *s_pattern = R"(^([a-zA-Z])((\d*)|_([a-zA-Z]*\d*)?)$)";

		string_type m_name;
	};
} // namespace mg
