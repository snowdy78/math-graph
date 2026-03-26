
#include <catch2/catch_test_macros.hpp>
#include "math_graph/function.hpp"
#include <cmath>

TEST_CASE("function construct", "[test]")
{
	SECTION("construct by string")
	{
		SECTION("correct name and args")
		{
			mg::function sqrt{ "f1(x)", [](const mg::map_dependencies &args) -> mg::function::return_type {
								  return std::sqrt(args.at({ "x" }));
							  } };
			REQUIRE(sqrt.fullname() == "f_1");
			REQUIRE(sqrt.args().size() == 1);
			REQUIRE(sqrt.args().contains({ "x" }));
		}
		SECTION("multiple args")
		{
			mg::function pow{ "f_a1(x1, x2)", [](const mg::map_dependencies &args) -> mg::function::return_type {
								 return std::pow(args.at({ "x1" }), args.at({ "x2" }));
							 } };
			REQUIRE(pow.fullname() == "f_a1");
			REQUIRE(pow.args().size() == 2);
			REQUIRE(pow.args().contains({ "x1" }));
			REQUIRE(pow.args().contains({ "x2" }));
		}
		SECTION("incorrect name")
		{
			REQUIRE_THROWS(mg::function{ "_f1(x)", [](const mg::map_dependencies &args) -> mg::function::return_type {
											return std::sqrt(args.at({ "x" }));
										} });

			REQUIRE_THROWS(mg::function{ "f1_1(x, x_b, x_c, y)",
										 [](const mg::map_dependencies &args) -> mg::function::return_type {
											 return std::sqrt(args.at({ "x" }));
										 } });
			REQUIRE_THROWS(mg::function{ "fa(x, x_b, x_c, y)",
										 [](const mg::map_dependencies &args) -> mg::function::return_type {
											 return std::sqrt(args.at({ "x" }));
										 } });
		}
		SECTION("incorrect args")
		{
			SECTION("no args")
			{
				REQUIRE_THROWS(mg::function{ "f1()", [](const mg::map_dependencies &args) -> mg::function::return_type {
												return std::sqrt(args.at({ "x" }));
											} });
			}
			SECTION("wrong args")
			{
				REQUIRE_THROWS(mg::function{ "f1(124121)",
											 [](const mg::map_dependencies &args) -> mg::function::return_type {
												 return std::sqrt(args.at({ "x" }));
											 } });
			}
			SECTION("same args")
			{
				REQUIRE_THROWS(mg::function{ "f1(x, x)",
											 [](const mg::map_dependencies &args) -> mg::function::return_type {
												 return std::sqrt(args.at({ "x" }));
											 } });
			}
			SECTION("comma without argument")
			{

				REQUIRE_THROWS(mg::function{ "f1(x, x, )",
											 [](const mg::map_dependencies &args) -> mg::function::return_type {
												 return std::sqrt(args.at({ "x" }));
											 } });

				REQUIRE_THROWS(mg::function{ "f1(, x, x)",
											 [](const mg::map_dependencies &args) -> mg::function::return_type {
												 return std::sqrt(args.at({ "x" }));
											 } });
			}
			SECTION("argument without comma")
			{
				REQUIRE_THROWS(mg::function{ "f1(a    c ,b)",
											 [](const mg::map_dependencies &args) -> mg::function::return_type {
												 return std::sqrt(args.at({ "x" }));
											 } });

				REQUIRE_THROWS(mg::function{ "f1(,,,)",
											 [](const mg::map_dependencies &args) -> mg::function::return_type {
												 return std::sqrt(args.at({ "x" }));
											 } });
			}
		}
	}
}
TEST_CASE("function call", "[test]")
{
	SECTION("correct args")
	{
		mg::function sqrt{ "f(x)", [](const mg::map_dependencies &args) -> mg::function::return_type {
							  return std::sqrt(args.at({ "x" }));
						  } };
		REQUIRE(
			sqrt({
				{ { "x" }, 4.0 }
		 })
			== 2.0
		);
	}
	SECTION("multiple arguments")
	{
		mg::function pow{ "f(x, n)", [](const mg::map_dependencies &args) -> mg::function::return_type {
							 return std::pow(args.at({ "x" }), args.at({ "n" }));
						 } };
		REQUIRE(
			pow({
				{ { "x" }, 4.0 },
				{ { "n" }, 2.0 }
		 })
			== 16.0
		);
	}
	SECTION("not all args was provided")
	{
		mg::function pow{ "f(x, n)", [](const mg::map_dependencies &args) -> mg::function::return_type {
							 return std::pow(args.at({ "x" }), args.at({ "n" }));
						 } };
		REQUIRE_THROWS(pow({
			{ { "x" }, 4.0 }
		 }));
	}
	SECTION("too much args was given")
	{
		mg::function pow{ "f(x, n)", [](const mg::map_dependencies &args) -> mg::function::return_type {
							 return std::pow(args.at({ "x" }), args.at({ "n" }));
						 } };
		REQUIRE_THROWS(pow({
			{ { "x" }, 4.0 },
			{ { "n" }, 2.0 },
			{ { "y" }, 3.0 }
		 }));
	}
}
