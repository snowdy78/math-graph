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
			mg::unary_operation op1{ "-y" };
			REQUIRE(op1.operation() == mg::unique_operations::minus);
			REQUIRE(std::holds_alternative<mg::independent_variable>(op1.operand()));
			REQUIRE(std::get<mg::independent_variable>(op1.operand()).fullname() == "y");
			REQUIRE(op1.deps().size() == 1);
			REQUIRE(op1.deps().contains({ "y" }));
		}
		SECTION("construct without operator")
		{
			mg::unary_operation op1{ "x" };
			REQUIRE(op1.operation() == mg::unique_operations::plus);
			REQUIRE(std::holds_alternative<mg::independent_variable>(op1.operand()));
			REQUIRE(std::get<mg::independent_variable>(op1.operand()).fullname() == "x");
			REQUIRE(op1.deps().size() == 1);
			REQUIRE(op1.deps().contains({ "x" }));
		}
		SECTION("construct by number")
		{
			mg::unary_operation op1{ "-2.0" };
			REQUIRE(op1.operation() == mg::unique_operations::minus);
			REQUIRE(std::holds_alternative<mg::number>(op1.operand()));
			REQUIRE(std::get<mg::number>(op1.operand()) == 2);
			REQUIRE(op1.deps().empty());
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
		SECTION("no expression")
		{
			REQUIRE_THROWS(mg::unary_operation{ "-(x + 2)" });
			REQUIRE_THROWS(mg::unary_operation{ "-x + 2" });
			REQUIRE_THROWS(mg::unary_operation{ "-f(x)" });
		}
		SECTION("wrong string")
		{
			REQUIRE_THROWS(mg::unary_operation{ "x-" });
		}
	}
}
