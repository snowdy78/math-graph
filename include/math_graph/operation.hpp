#pragma once

#include "mgraphfwd.hpp"
#include "number.hpp"
#include <stdexcept>
#include <unordered_map>
#include <functional>

namespace mg
{

	class operation
	{
	public:
		using key_type		   = char_type;
		using function_type	   = std::function<number(const std::vector<number> &)>;
		using compute_map_type = std::unordered_map<const operation *, function_type>;

	private:
		constexpr static const char *s_pattern = R"(^[\*\-\+\/\^]$)";
		constexpr static key_type parse(const key_type &op)
		{
			std::string allowed(s_pattern);
			if (allowed.find(op) == std::string::npos)
			{
				throw std::runtime_error("unknown operation syntax");
			}
			return op;
		}
		constexpr operation(const key_type &op)
			: m_name(parse(op))
		{}
		operation(const operation &other) = default;
		operation(operation &&other)	  = default;

	public:
		constexpr key_type get() const
		{
			return m_name;
		}
		static const compute_map_type &get_compute_map();
		static const operation &get_by_name(const key_type &);
		bool operator==(const operation &other) const;

	private:
		key_type m_name;
		friend class unique_operations;
	};
	struct unique_operations
	{
		constexpr static operation add{ '+' };
		constexpr static operation sub{ '-' };
		constexpr static operation mul{ '*' };
		constexpr static operation div{ '/' };
		constexpr static operation pow{ '^' };
		constexpr static operation plus{ '+' };	 // unary plus
		constexpr static operation minus{ '-' }; // unary minus
		~unique_operations() = delete;
	}; // namespace defined_ops

} // namespace mg
