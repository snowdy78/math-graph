#include <catch2/catch_test_macros.hpp>
#include "math_graph/operation.hpp"
#include "math_graph/unary_operator_action.hpp"
#include "math_graph/unpack_variable_action.hpp"

TEST_CASE("unary operation construct", "[test]")
{
	using uoperator = mg::unary_operator_action;
	auto check
		= []<class... T>(const uoperator &act, const mg::unary_operation &op, size_t dep_count, const T &...dep_names) {
			  REQUIRE(&act.operation() == &op);
			  REQUIRE(act.deps().size() == dep_count);
			  REQUIRE((act.deps().contains(dep_names) && ...));
		  };
	SECTION("construct by correct string")
	{
		SECTION("construct by variable")
		{
			mg::unary_operator_action op1{ "-y" };
			check(op1, mg::unique_operations::minus, 1, "y");
			REQUIRE(op1.operand().is_expression());
			auto p = dynamic_cast<const mg::unpack_variable_action *>(&op1.operand().as_expression());
			REQUIRE(p != nullptr);
			REQUIRE(p->variable().fullname() == "y");
		}
		SECTION("construct without operator")
		{
			mg::unary_operator_action op1{ "x" };
			check(op1, mg::unique_operations::plus, 1, "x");
			REQUIRE(op1.operand().is_expression());
			auto p = dynamic_cast<const mg::unpack_variable_action *>(&op1.operand().as_expression());
			REQUIRE(p != nullptr);
			REQUIRE(p->variable().fullname() == "x");
		}
		SECTION("construct by number")
		{
			mg::unary_operator_action op1{ "-2.0" };
			check(op1, mg::unique_operations::minus, 0);
			REQUIRE(op1.operand().is_number());
			REQUIRE(op1.operand().as_number() == 2);
			REQUIRE(op1.deps().empty());
		}
		REQUIRE_NOTHROW(mg::unary_operator_action{ "- x" });
		REQUIRE_NOTHROW(mg::unary_operator_action{ "  -  x   " });
	}
	SECTION("construct by incorrect string")
	{
		SECTION("empty string")
		{
			REQUIRE_THROWS(mg::unary_operator_action{ "" });
		}
		SECTION("wrong string")
		{
			REQUIRE_THROWS(mg::unary_operator_action{ "a@43$FSA_fas" });
		}
		SECTION("no expression")
		{
			REQUIRE_THROWS(mg::unary_operator_action{ "-(x + 2)" });
			REQUIRE_THROWS(mg::unary_operator_action{ "-x + 2" });
			REQUIRE_THROWS(mg::unary_operator_action{ "-f(x)" });
		}
		SECTION("wrong string")
		{
			REQUIRE_THROWS(mg::unary_operator_action{ "x-" });
		}
	}
}
