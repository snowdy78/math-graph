#include <catch2/catch_test_macros.hpp>
#include "math_graph/call_function_action.hpp"
#include <cmath>

TEST_CASE("function call", "[test]")
{
	using fdef = mg::definition<mg::function_declaration>;
	auto check_result
		= [](const mg::call_function_action &func, mg::number n = 0.0, const mg::dependency_map &deps = {}) {
			  auto result = func.evaluate(deps);
			  REQUIRE(result == n);
		  };
	SECTION("correct args")
	{
		mg::expression x{ 4.0 };
		mg::call_function_action sqrt{
			fdef{ { "f(x)" }, mg::expression([](const mg::dependency_map &args) -> mg::expression::result_type {
					  return std::sqrt(args.at("x")->evaluate(args));
				  }) },
			{ &x }
		};
		check_result(sqrt, 2.0);
	}
	SECTION("multiple arguments")
	{
		mg::expression x{ 4.0 };
		mg::expression n{ 2.0 };
		mg::call_function_action pow{
			fdef{ { "f(x, n)" }, mg::expression([](const mg::dependency_map &args) -> mg::expression::result_type {
					  return std::pow(args.at({ "x" })->evaluate(args), args.at({ "n" })->evaluate(args));
				  }) },
			{ &x,			  &n					 }
		};
		check_result(pow, 16.0);
	}
	SECTION("not all args was provided")
	{
		mg::expression x{ 4.0 };
		REQUIRE_THROWS(mg::call_function_action{
			fdef{ { "f(x, n)" }, mg::expression([](const mg::dependency_map &args) -> mg::expression::result_type {
					  return std::pow(args.at({ "x" })->evaluate(args), args.at({ "n" })->evaluate(args));
				  }) },
			{
				 &x,
				 }
		  });
	}
	SECTION("too much args was given")
	{
		mg::expression x{ 4.0 }, n{ 4.0 }, y{ 4.0 };
		REQUIRE_THROWS(mg::call_function_action{
			fdef{ { "f(x, n)" }, mg::expression([](const mg::dependency_map &args) -> mg::expression::result_type {
					  return std::pow(args.at({ "x" })->evaluate(args), args.at({ "n" })->evaluate(args));
				  }) },
			{ &x, &n, &y }
		   });
	}
}
