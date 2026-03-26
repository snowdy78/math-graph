#pragma once
#include <catch2/catch_test_macros.hpp>
#include "math_graph/operation.hpp"
#include "math_graph/unary_operation.hpp"

TEST_CASE("unary operation construct", "[test]")
{
	SECTION("construct by correct string")
	{
		SECTION("construct by variable")
		{
			mg::unary_operation op1{ "-x" };
			REQUIRE(op1.operation() == mg::unique_operations::minus);
			REQUIRE(std::holds_alternative<mg::independent_variable>(op1.operand()));
			REQUIRE(std::get<mg::independent_variable>(op1.operand()).fullname() == "x");
		}
		SECTION("construct by function")
		{
			mg::unary_operation op1{ "+f_a1(x, y)" };
			REQUIRE(op1.operation() == mg::unique_operations::plus);
			REQUIRE(std::holds_alternative<mg::unexpressed_function>(op1.operand()));
			auto &f = std::get<mg::unexpressed_function>(op1.operand());
			REQUIRE(f.fullname() == "f_a1");
			REQUIRE(f.args().size() == 2);
			REQUIRE(f.args().contains({ "x" }));
			REQUIRE(f.args().contains({ "y" }));
		}
		SECTION("construct by number")
		{
			mg::unary_operation op1{ "-2.0" };
			REQUIRE(op1.operation() == mg::unique_operations::minus);
			REQUIRE(std::holds_alternative<mg::number>(op1.operand()));
			REQUIRE(std::get<mg::number>(op1.operand()) == 2);
		}
		REQUIRE_NOTHROW(mg::unary_operation{ "- x" });
		REQUIRE_NOTHROW(mg::unary_operation{ "  -  x   " });
	}
	SECTION("construct by incorrect string")
	{
		SECTION("empty string")
		{
			REQUIRE_THROWS(mg::unary_operation{ "" });
		}
		SECTION("wrong string")
		{
			REQUIRE_THROWS(mg::unary_operation{ "a@43$FSA_fas" });
		}
		SECTION("wrong string")
		{
			REQUIRE_THROWS(mg::unary_operation{ "x-" });
		}
	}
}
