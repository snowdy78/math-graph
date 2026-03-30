#include "catch2/catch_test_macros.hpp"
#include "math_graph/binary_operator_action.hpp"
#include "math_graph/action.hpp"

TEST_CASE("binary_operator_action construct", "[test]")
{
	SECTION("construct by correct string")
	{
		SECTION("two numbers")
		{
			mg::binary_operator_action op{ "1 + 2" };
			REQUIRE(&op.op() == &mg::unique_operations::add);
			REQUIRE(op.deps().size() == 0);
			REQUIRE(std::holds_alternative<mg::binary_operator_action::number_type>(op.left()));
			REQUIRE(std::holds_alternative<mg::binary_operator_action::number_type>(op.right()));
			REQUIRE(std::get<mg::binary_operator_action::number_type>(op.left()) == 1.0);
			REQUIRE(std::get<mg::binary_operator_action::number_type>(op.right()) == 2.0);
		}
		SECTION("var and number")
		{
			mg::binary_operator_action op{ "x + 2" };
			REQUIRE(&op.op() == &mg::unique_operations::add);
			REQUIRE(op.deps().size() == 1);
			REQUIRE(op.deps().contains({ "x" }));
			REQUIRE(std::holds_alternative<mg::binary_operator_action::variable_type>(op.left()));
			REQUIRE(std::holds_alternative<mg::binary_operator_action::number_type>(op.right()));
			REQUIRE(std::get<mg::binary_operator_action::number_type>(op.right()) == 2.0);
		}
		SECTION("var and var")
		{
			mg::binary_operator_action op{ "x + x1" };
			REQUIRE(&op.op() == &mg::unique_operations::add);
			REQUIRE(op.deps().size() == 2);
			REQUIRE(op.deps().contains({ "x" }));
			REQUIRE(op.deps().contains({ "x1" }));
			REQUIRE(std::holds_alternative<mg::binary_operator_action::variable_type>(op.left()));
			REQUIRE(std::holds_alternative<mg::binary_operator_action::variable_type>(op.right()));
		}
		SECTION("with brackets")
		{
			SECTION("for numbers")
			{
				auto check_parsing = [](mg::string_type str, mg::number n1, mg::number n2) {
					mg::binary_operator_action op{ str };
					REQUIRE(&op.op() == &mg::unique_operations::add);
					REQUIRE(op.deps().size() == 0);
					REQUIRE(std::holds_alternative<mg::binary_operator_action::number_type>(op.left()));
					REQUIRE(std::holds_alternative<mg::binary_operator_action::number_type>(op.right()));
					REQUIRE(std::get<mg::binary_operator_action::number_type>(op.left()) == n1);
					REQUIRE(std::get<mg::binary_operator_action::number_type>(op.right()) == n2);
				};
				check_parsing("-1+2.0", -1.0, 2.0);
				check_parsing("1+(-1)", 1.0, -1.0);
				check_parsing("1+(+1)", 1.0, 1.0);
				check_parsing("1+(1)", 1.0, 1.0);
				check_parsing("(-1) + (-1.0)", -1.0, -1.0);
				check_parsing("(1) + (1.0)", 1.0, 1.0);
				check_parsing("-1.0 + (1)", -1.0, 1.0);
				REQUIRE_THROWS(mg::binary_operator_action{ "1 - -1" });
				REQUIRE_THROWS(mg::binary_operator_action{ "1--1" });
				REQUIRE_THROWS(mg::binary_operator_action{ "1+-1" });
			}
			SECTION("for vars")
			{
				auto check_parsing = [](mg::string_type str, mg::string_type name1, mg::string_type name2) {
					mg::binary_operator_action op{ str };
					REQUIRE(&op.op() == &mg::unique_operations::add);
					REQUIRE(op.deps().size() == 2);
					REQUIRE(op.deps().contains({ name1 }));
					REQUIRE(op.deps().contains({ name2 }));
					REQUIRE(std::holds_alternative<mg::binary_operator_action::variable_type>(op.left()));
					REQUIRE(std::holds_alternative<mg::binary_operator_action::variable_type>(op.right()));
				};
				check_parsing("(x1) + (x2)", "x1", "x2");
				check_parsing("x1+(x2)", "x1", "x2");
				check_parsing("(x1)+a2", "x1", "a2");
				REQUIRE_THROWS(mg::binary_operator_action{ "+x1 + (x3)" });
				REQUIRE_THROWS(mg::binary_operator_action{ "(-x1) + (-x2)" });
				REQUIRE_THROWS(mg::binary_operator_action{ "-x1 - x2" });
				REQUIRE_THROWS(mg::binary_operator_action{ "x1+(+x2)" });
				REQUIRE_THROWS(mg::binary_operator_action{ "x1 - -x2" });
				REQUIRE_THROWS(mg::binary_operator_action{ "x1--x2" });
				REQUIRE_THROWS(mg::binary_operator_action{ "x1+-x2" });
			}
		}
	}
	SECTION("construct by incorrect string")
	{
		SECTION("empty string")
		{
			REQUIRE_THROWS(mg::binary_operator_action{ "" });
		}
		SECTION("wrong string")
		{
			REQUIRE_THROWS(mg::binary_operator_action{ "a@43$FSA_fas" });
		}
		SECTION("too many operations")
		{
			REQUIRE_THROWS(mg::binary_operator_action{ "1 + 2 + 3" });
		}
		SECTION("wrong operands")
		{
			REQUIRE_THROWS(mg::binary_operator_action{ "1 + 2a" });
			REQUIRE_THROWS(mg::binary_operator_action{ "$fa + 2" });
		}
		SECTION("less operands")
		{
			REQUIRE_THROWS(mg::binary_operator_action{ "1 + " });
			REQUIRE_THROWS(mg::binary_operator_action{ "+" });
			REQUIRE_THROWS(mg::binary_operator_action{ "- 1" });
		}
		SECTION("wrong operation")
		{
			REQUIRE_THROWS(mg::binary_operator_action{ "1 _ 2" });
			REQUIRE_THROWS(mg::binary_operator_action{ "1 a 2" });
		}
		SECTION("negative var")
		{
			REQUIRE_THROWS(mg::binary_operator_action{ "-x + 1" });
			REQUIRE_NOTHROW(mg::binary_operator_action{ "+1 - x" });
		}
	}
	SECTION("construct by values")
	{
		SECTION("two numbers")
		{
			mg::binary_operator_action op{ 1.0, mg::unique_operations::add, 2.0 };
			REQUIRE(&op.op() == &mg::unique_operations::add);
			REQUIRE(op.deps().size() == 0);
			REQUIRE(std::holds_alternative<mg::binary_operator_action::number_type>(op.left()));
			REQUIRE(std::holds_alternative<mg::binary_operator_action::number_type>(op.right()));
			REQUIRE(std::get<mg::binary_operator_action::number_type>(op.left()) == 1.0);
			REQUIRE(std::get<mg::binary_operator_action::number_type>(op.right()) == 2.0);
		}

		SECTION("number and var")
		{
			mg::binary_operator_action::variable_type x{ "x" };
			mg::binary_operator_action op{ 1.0, mg::unique_operations::add, x };
			REQUIRE(&op.op() == &mg::unique_operations::add);
			REQUIRE(op.deps().size() == 1);
			REQUIRE(op.deps().contains({ "x" }));
			REQUIRE(std::holds_alternative<mg::binary_operator_action::number_type>(op.left()));
			REQUIRE(std::holds_alternative<mg::binary_operator_action::variable_type>(op.right()));
			REQUIRE(std::get<mg::binary_operator_action::number_type>(op.left()) == 1.0);
		}
		SECTION("var and number")
		{
			mg::binary_operator_action::variable_type x{ "x" };
			mg::binary_operator_action op{ x, mg::unique_operations::add, 1.0 };
			REQUIRE(&op.op() == &mg::unique_operations::add);
			REQUIRE(op.deps().size() == 1);
			REQUIRE(op.deps().contains({ "x" }));
			REQUIRE(std::holds_alternative<mg::binary_operator_action::variable_type>(op.left()));
			REQUIRE(std::holds_alternative<mg::binary_operator_action::number_type>(op.right()));
			REQUIRE(std::get<mg::binary_operator_action::number_type>(op.right()) == 1.0);
		}
		SECTION("var and var")
		{
			mg::binary_operator_action::variable_type x{ "x" }, x1{ "x1" };
			mg::binary_operator_action op{ x, mg::unique_operations::add, x1 };
			REQUIRE(&op.op() == &mg::unique_operations::add);
			REQUIRE(op.deps().size() == 2);
			REQUIRE(op.deps().contains({ "x" }));
			REQUIRE(op.deps().contains({ "x1" }));
			REQUIRE(std::holds_alternative<mg::binary_operator_action::variable_type>(op.left()));
			REQUIRE(std::holds_alternative<mg::binary_operator_action::variable_type>(op.right()));
		}
		SECTION("except wrong provided operator action")
		{
			REQUIRE_THROWS(mg::binary_operator_action{ 1.0, mg::unique_operations::add, nullptr });
		}
		SECTION("number and other operation")
		{
			mg::action p{ mg::binary_operator_action{ "x + 2" } };
			mg::binary_operator_action op{ 1.0, mg::unique_operations::add, &p };
			REQUIRE(&op.op() == &mg::unique_operations::add);
			REQUIRE(op.deps().size() == 1);
			REQUIRE(op.deps().contains({ "x" }));
			REQUIRE(std::holds_alternative<mg::binary_operator_action::number_type>(op.left()));
			REQUIRE(std::holds_alternative<mg::binary_operator_action::pointer_type>(op.right()));
			REQUIRE(std::get<mg::binary_operator_action::number_type>(op.left()) == 1.0);
		}
		SECTION("var and other operation")
		{
			auto test = [](mg::string_type opact_string, mg::string_type var_name) {
				mg::action p{ mg::binary_operator_action{ opact_string } };
				mg::independent_variable x{ var_name };
				mg::binary_operator_action op{ x, mg::unique_operations::add, &p };
				REQUIRE(&op.op() == &mg::unique_operations::add);
				REQUIRE(std::holds_alternative<mg::binary_operator_action::variable_type>(op.left()));
				REQUIRE(std::holds_alternative<mg::binary_operator_action::pointer_type>(op.right()));
				return op;
			};
			SECTION("with same vars")
			{
				auto op = test("x + 2", "x");
				REQUIRE(op.deps().size() == 1);
				REQUIRE(op.deps().contains({ "x" }));
			}
			SECTION("with different vars")
			{
				auto op = test("x + 2", "y");
				REQUIRE(op.deps().size() == 2);
				REQUIRE(op.deps().contains({ "x" }));
				REQUIRE(op.deps().contains({ "y" }));
			}
			SECTION("with many different vars")
			{
				auto op = test("x + z", "y");
				REQUIRE(op.deps().size() == 3);
				REQUIRE(op.deps().contains({ "x" }));
				REQUIRE(op.deps().contains({ "y" }));
				REQUIRE(op.deps().contains({ "z" }));
			}
		}
		SECTION("operation and other operation")
		{
			auto test = [](mg::string_type opact_string, mg::string_type other_opact_string) {
				mg::action p{ mg::binary_operator_action{ opact_string } },
					x{ mg::binary_operator_action{ other_opact_string } };
				mg::binary_operator_action op{ &x, mg::unique_operations::add, &p };
				REQUIRE(&op.op() == &mg::unique_operations::add);
				REQUIRE(std::holds_alternative<mg::binary_operator_action::pointer_type>(op.left()));
				REQUIRE(std::holds_alternative<mg::binary_operator_action::pointer_type>(op.right()));
				return op;
			};
			SECTION("with same vars")
			{
				auto op = test("x + x", "x + 3");
				REQUIRE(op.deps().size() == 1);
				REQUIRE(op.deps().contains({ "x" }));
			}
			SECTION("with different vars")
			{
				auto op = test("x + 2", "y + 3");
				REQUIRE(op.deps().size() == 2);
				REQUIRE(op.deps().contains({ "x" }));
				REQUIRE(op.deps().contains({ "y" }));
			}
			SECTION("with many different vars")
			{
				auto op = test("x + z", "y + w");
				REQUIRE(op.deps().size() == 4);
				REQUIRE(op.deps().contains({ "x" }));
				REQUIRE(op.deps().contains({ "y" }));
				REQUIRE(op.deps().contains({ "z" }));
				REQUIRE(op.deps().contains({ "w" }));
			}
		}
	}
}
