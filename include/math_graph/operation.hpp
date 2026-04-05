#pragma once

#include "mgraphfwd.hpp"
#include "number.hpp"
#include <stdexcept>
#include <unordered_map>
#include <functional>
#include <array>
#include "regex_tools.hpp"
#include <cmath>

namespace mg
{
	template<class CharType, class RetT, class... Args>
	struct basic_operation;
	template<class CharType, class RetT, class... Args>
	struct basic_operation<CharType, RetT(Args...)>
	{
	public:
		using function_type		  = std::function<RetT(Args...)>;
		using key_type			  = CharType;
		using value_type		  = basic_operation;
		using operations_map_type = std::unordered_map<key_type, const value_type *>;

	private:
		constexpr static const char *s_operations = R"(\+\-\*\/\^)";
		constexpr static const char *rgx1		  = "^[";
		constexpr static const char *rgx2		  = "]$";
		constexpr static auto get_pattern()
		{
			constexpr size_t N1 = std::char_traits<CharType>::length(rgx1);
			constexpr size_t N2 = std::char_traits<CharType>::length(s_operations);
			constexpr size_t N3 = std::char_traits<CharType>::length(rgx2);
			return concat<N1 + N2, N3>(concat<N1, N2>(rgx1, s_operations).data(), rgx2);
		}
		constexpr static std::array s_pattern = get_pattern();

		static string_type parse(const string_type &op)
		{
			std::regex allowed(s_pattern.data());
			std::smatch match;
			if (!std::regex_search(op, match, allowed))
			{
				throw std::runtime_error("unknown operation syntax");
			}
			return match[0].str();
		}

	protected:
		basic_operation(const key_type &op, const size_t &priority, const function_type &function)
			: m_name(parse(std::string(1, op))[0]),
			  m_priority(priority),
			  m_function(function)
		{
			operations().emplace(m_name, this);
		}
		~basic_operation()
		{
			operations().erase(m_name);
		}
		basic_operation(const basic_operation &other)				 = default;
		basic_operation(basic_operation &&other) noexcept			 = default;
		basic_operation &operator=(const basic_operation &other)	 = default;
		basic_operation &operator=(basic_operation &&other) noexcept = default;
		static operations_map_type &operations()
		{
			static operations_map_type map;
			return map;
		}

	public:
		key_type get() const
		{
			return m_name[0];
		}
		size_t priority() const
		{
			return m_priority;
		}
		static const value_type &get_by_name(const key_type &op)
		{
			return *operations().at(op);
		}
		RetT operator()(Args... args) const
		{
			return m_function(args...);
		}

	private:
		key_type m_name;
		size_t m_priority = 0;
		function_type m_function;
		friend struct unique_operations;
	};
	template<class Ret, class... Args>
	using operation = basic_operation<char_type, Ret, Args...>;

	using binary_operation = operation<number(const number &, const number &)>;
	using unary_operation  = operation<number(const number &)>;
	struct unique_operations
	{
		inline static const binary_operation add{ '+', 1, [](const number &a, const number &b) -> number {
													 return a + b;
												 } };
		inline static const binary_operation sub{ '-', 1, [](const number &a, const number &b) -> number {
													 return a - b;
												 } };
		inline static const binary_operation mul{ '*', 2, [](const number &a, const number &b) -> number {
													 return a * b;
												 } };
		inline static const binary_operation div{ '/', 2, [](const number &a, const number &b) -> number {
													 return a / b;
												 } };
		inline static const binary_operation pow{ '^', 3, [](const number &a, const number &b) -> number {
													 return std::pow(a, b);
												 } };
		inline static const unary_operation plus{ '+', 2, [](const number &a) -> number {
													 return +a;
												 } }; // unary plus
		inline static const unary_operation minus{ '-', 2, [](const number &a) -> number {
													  return -a;
												  } }; // unary minus
		~unique_operations() = delete;
	}; // namespace defined_ops

} // namespace mg
