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
		mg::call_function_action sqrt{
			fdef{ { "f(x)" }, mg::expression([](const mg::dependency_map &args) -> mg::expression::result_type {
					  return std::sqrt(args.at("x")->evaluate(args));
				  }) },
			{ { 4.0 } }
		};
		check_result(sqrt, 2.0);
	}
	SECTION("multiple arguments")
	{
		mg::call_function_action pow{
			fdef{ { "f(x, n)" }, mg::expression([](const mg::dependency_map &args) -> mg::expression::result_type {
					  return std::pow(args.at({ "x" })->evaluate(args), args.at({ "n" })->evaluate(args));
				  }) },
			{ { 4.0 },	   { 2.0 }			   }
		};
		check_result(pow, 16.0);
	}
	SECTION("not all args was provided")
	{
		REQUIRE_THROWS(mg::call_function_action{
			fdef{ { "f(x, n)" }, mg::expression([](const mg::dependency_map &args) -> mg::expression::result_type {
					  return std::pow(args.at({ "x" })->evaluate(args), args.at({ "n" })->evaluate(args));
				  }) },
			{
				 { 4.0 },
				 }
		  });
	}
	SECTION("too much args was given")
	{
		REQUIRE_THROWS(mg::call_function_action{
			fdef{ { "f(x, n)" }, mg::expression([](const mg::dependency_map &args) -> mg::expression::result_type {
					  return std::pow(args.at({ "x" })->evaluate(args), args.at({ "n" })->evaluate(args));
				  }) },
			{ { 4.0 }, { 4.0 }, { 4.0 } }
		  });
	}
}
