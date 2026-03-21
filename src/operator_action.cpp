#include "math_graph/tools.hpp"

namespace mg
{
	void operator_action::parse(const string_type &str)
	{
		std::regex op_rgx(s_operator_action_pattern);
		std::smatch match;
		if (!std::regex_search(str, match, op_rgx))
		{
			throw std::runtime_error("unable to create expression by '" + str + "'");
		}
		string_type words[]
			= { match[2].str().empty() ? match[3] : match[2], match[4], match[6].str().empty() ? match[7] : match[6] };
		m_left = create_parameter_data(words[0]);
		if (std::holds_alternative<independent_variable>(*m_left))
		{
			m_deps.insert(std::get<independent_variable>(*m_left));
		}
		m_op	= &operation::get_by_name(words[1][0]);
		m_right = create_parameter_data(words[2]);
		if (std::holds_alternative<independent_variable>(*m_right))
		{
			m_deps.insert(std::get<independent_variable>(*m_right));
		}
	}
} // namespace mg
