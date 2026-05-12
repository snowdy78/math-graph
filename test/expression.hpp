#include <catch2/catch_test_macros.hpp>
#include "math_graph/dependency_map.hpp"
#include "math_graph/expression.hpp"
#include "math_graph/call_function_action.hpp"
#include "math_graph/binary_operator_action.hpp"
#include "math_graph/operation.hpp"
#include "math_graph/unary_operator_action.hpp"

TEST_CASE("expression construct", "[test]")
{
	auto check_expr = [](const mg::expression &expr, const mg::expression::result_type &expected,
						 bool has_solution = true, const mg::dependency_map &values = {}) {
		REQUIRE(expr.has_solution(values) == has_solution);
		REQUIRE(std::all_of(values.begin(), values.end(), [&](const auto &dep) {
			return expr.deps().contains(dep.first);
		}));
		if (has_solution)
		{
			REQUIRE(expr.evaluate(values) == expected);
		}
	};
	SECTION("construct by number")
	{
		mg::expression expr{ 2.0 };
		check_expr(expr, 2.0);
	}
	// TODO fix this test and maybe remove creating expression by a function
	// SECTION("construct by function")
	// {
	// 	mg::expression expr{ [](const mg::dependency_map &args) -> mg::expression::result_type {
	// 		return args.at("x")->evaluate(args);
	// 	} };
	// 	mg::expression x{ 2.0 };
	// 	check_expr(expr, 2.0, true);
	// }
	SECTION("construct by string")
	{
		auto check_str = [&check_expr](
							 const mg::string_type &str, const mg::function_dependencies &func_deps = {},
							 const mg::expression::result_type &expected = 0.0, bool has_solution = true,
							 const mg::dependency_map &values = {}
						 ) {
			CAPTURE(str);
			CAPTURE(func_deps);
			check_expr(mg::expression{ str, func_deps }, expected, has_solution, values);
		};
		SECTION("empty string")
		{
			REQUIRE_THROWS(mg::expression{ "" });
		}
		SECTION("arithmetic")
		{
			SECTION("simple number")
			{
				mg::expression expr{ "2.0" };
				check_str("2.0", {}, 2.0);
			}
			SECTION("actions with numbers")
			{
				check_str("-2.0 - 1", {}, -3.0);
			}
			SECTION("difficult expression with numbers")
			{
				check_str("-2 + 5. * (2.2 + 3.0)", {}, 24.0, true);
			}
			SECTION("actions with variables")
			{
				mg::expression x{ 2.0 };
				check_str(
					"-x + 1.0",
					{
				},
					-1.0, true, mg::dependency_map{ { "x", &x } }
				);
			}
			SECTION("difficult expression with variables")
			{
				mg::expression x{ 2.0 };
				mg::expression y{ 3.0 };
				check_str(
					"-x + 5. * (2.2 + y)",
					{
				},
					24.0, true, mg::dependency_map{ { "x", &x }, { "y", &y } }
				);
			}
			SECTION("simple function")
			{
				mg::expression x{ 2.0 };
				check_str(
					"f(x)",
					{
						{ "f", { { "f(x)" }, { "x * 2.0" } } }
				   },
					4.0, true, { { "x", &x } }
				);
			}
			SECTION("function with difficult args")
			{
				mg::expression x{ 2.0 }, y{ 3.0 };
				check_str(
					"f(x*2, y*(2 - x/2))",
					{
						{ "f", { { "f(x, y)" }, { "x + y" } } }
				},
					7.0, true, { { "x", &x }, { "y", &y } }
				);
			}
			SECTION("function with action")
			{
				mg::expression x{ 2.0 }, y{ 3.0 };
				check_str(
					"-f(x) + 2*y",
					{
						{ "f", { { "f(x)" }, { "x * 2.0" } } }
				   },
					2.0, true, { { "x", &x }, { "y", &y } }
				);
			}
			SECTION("difficult expression")
			{
				mg::expression x{ 2.0 }, y{ 3.0 };
				check_str(
					"-f((x-1)*(x-3)) + 2*y - (-x) - y^x - (x + y) / (x - y)",
					{
						{ "f", { { "f(x)" }, { "x * 2.0" } } }
				   },
					6.0, true, { { "x", &x }, { "y", &y } }
				);
			}
		}
	}
	SECTION("construct by other action")
	{
		SECTION("function")
		{
			mg::expression expr(mg::call_function_action(
				mg::definition<mg::function_declaration>{
					{ "f(x)" },
					mg::expression([](const mg::dependency_map &args) {
						return args.at("x")->evaluate(args) * 2.0;
					}),
				},
				{ { 2.0 } }
			));
			check_expr(expr, 4.0);
		}
		SECTION("binary operator")
		{
			mg::expression expr(mg::binary_operator_action({ 2.0 }, mg::unique_operations::add, { 2.0 }));
			check_expr(expr, 4.0);
		}
		SECTION("unary operator")
		{
			mg::expression expr(mg::unary_operator_action(mg::unique_operations::minus, { 2.0 }));
			check_expr(expr, -2.0);
		}
		SECTION("inner expression")
		{
			mg::expression expr(mg::binary_operator_action(
				{ 2.0 }, mg::unique_operations::add, mg::unary_operator_action(mg::unique_operations::minus, { 2.0 })
			));
			check_expr(expr, 0.0);
		}
	}
}
