#pragma once

#include "mgraphfwd.hpp"
#include "number.hpp"
#include <stdexcept>
#include <unordered_map>
#include <functional>
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
		using string_type		  = std::basic_string<CharType>;

	private:
		struct operation_receiver
		{
			operation_receiver() {}
			void insert(const CharType &op)
			{
				if (m_operations.find(op) != string_type::npos)
				{
					throw std::runtime_error("operation_regex_pattern: operation already exists");
				}
				m_operations += string_type{ "\\" } + op;
				m_state_updated = true;
			}
			string_type open_pattern() const
			{
				return m_operations.empty() ? "." : ("[" + m_operations + "]");
			}
			const string_type &pattern() const
			{
				if (m_state_updated)
				{
					m_pattern		= "^" + open_pattern() + "$";
					m_state_updated = false;
				}
				return m_pattern;
			}
			const string_type &operations() const
			{
				return m_operations;
			}

		private:
			string_type m_operations;
			mutable string_type m_pattern;
			mutable bool m_state_updated = true;
		};
		static operation_receiver &pattern()
		{
			static operation_receiver pattern;
			return pattern;
		}
		inline static const char *s_operator_pattern = R"(^\W$)";
		static key_type parse(const key_type &op)
		{
			std::basic_regex<key_type> rgx(s_operator_pattern);
			if (!std::regex_search(string_type(1, op), rgx))
			{
				throw std::runtime_error("invalid name of operator. unable to parse");
			}
			pattern().insert(op);
			return op;
		}

	protected:
		basic_operation(const key_type &op, const size_t &priority, const function_type &function)
			: m_name(parse(op)),
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
		static const string_type &string_operations()
		{
			return pattern().operations();
		} // for regex operations
		static string_type open_pattern()
		{
			return pattern().open_pattern();
		} // for regex
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
