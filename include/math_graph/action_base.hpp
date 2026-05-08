#pragma once

#include "dependency_map.hpp"
#include "number.hpp"
namespace mg
{
	class action_base : public dependent
	{
	protected:
		/**
		 * @brief replace all need names from raw pattern to specific vars
		 *
		 * @param raw_pattern
		 * @param replacements - key is name to be replaced, value is insert value
		 * @return string_type
		 */
		static string_type
		get_pattern(string_type raw_pattern, const std::unordered_map<string_type, string_type> &replacements)
		{
			for (auto &[key, value]: replacements)
			{
				for (auto i = raw_pattern.find(key); i != string_type::npos; i = raw_pattern.find(key))
				{
					raw_pattern.replace(i, key.size(), value);
				}
			}
			return raw_pattern;
		}

	public:
		using result_type	= number;
		using unique_action = std::unique_ptr<action_base>;

		virtual ~action_base()											 = default;
		virtual size_t priority() const									 = 0;
		virtual unique_action copy() const								 = 0;
		virtual result_type evaluate(const dependency_map &values) const = 0;
	};
} // namespace mg
