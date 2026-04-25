#include "catch2/catch_test_macros.hpp"
#include "math_graph/binary_operator_action.hpp"
#include "math_graph/unpack_variable_action.hpp"
#include "math_graph/variable_declaration.hpp"

TEST_CASE("binary_operator_action construct", "[test]")
{
	SECTION("construct by correct string")
	{
		SECTION("two numbers")
		{
			mg::binary_operator_action op{ "1 + 2" };
			REQUIRE(&op.operation() == &mg::unique_operations::add);
			REQUIRE(op.deps().size() == 0);
			REQUIRE(op.left().is_number());
			REQUIRE(op.right().is_number());
			REQUIRE(op.left().as_number() == 1.0);
			REQUIRE(op.right().as_number() == 2.0);
		}
		SECTION("var and number")
		{
			mg::binary_operator_action op{ "x + 2" };
			REQUIRE(&op.operation() == &mg::unique_operations::add);
			REQUIRE(op.deps().size() == 1);
			REQUIRE(op.deps().contains({ "x" }));
			REQUIRE(op.left().is_expression());
			REQUIRE(op.right().is_number());
			auto p = dynamic_cast<const mg::unpack_variable_action *>(&op.left().as_expression());
			REQUIRE(p != nullptr);
			REQUIRE(p->variable().fullname() == "x");
			REQUIRE(op.right().as_number() == 2.0);
		}
		SECTION("var and var")
		{
			mg::binary_operator_action op{ "x + x1" };
			REQUIRE(&op.operation() == &mg::unique_operations::add);
			REQUIRE(op.deps().size() == 2);
			REQUIRE(op.deps().contains({ "x" }));
			REQUIRE(op.deps().contains({ "x_1" }));
			auto pl = dynamic_cast<const mg::unpack_variable_action *>(&op.left().as_expression());
			REQUIRE(pl != nullptr);
			REQUIRE(pl->variable().fullname() == "x");

			auto pr = dynamic_cast<const mg::unpack_variable_action *>(&op.right().as_expression());
			REQUIRE(pr != nullptr);
			REQUIRE(pr->variable().fullname() == "x_1");
		}
		SECTION("with brackets")
		{
			SECTION("for numbers")
			{
				auto check_parsing = [](mg::string_type str, mg::number n1, mg::number n2) {
					mg::binary_operator_action op{ str };
					REQUIRE(&op.operation() == &mg::unique_operations::add);
					REQUIRE(op.deps().size() == 0);
					REQUIRE(op.left().is_number());
					REQUIRE(op.right().is_number());
					REQUIRE(op.left().as_number() == n1);
					REQUIRE(op.right().as_number() == n2);
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
					REQUIRE(&op.operation() == &mg::unique_operations::add);
					REQUIRE(op.deps().size() == 2);
					CAPTURE(name1, name2);
					REQUIRE(op.deps().contains({ name1 }));
					REQUIRE(op.deps().contains({ name2 }));
					REQUIRE(op.left().is_expression());
					REQUIRE(op.right().is_expression());
					auto pl = dynamic_cast<const mg::unpack_variable_action *>(&op.left().as_expression());
					REQUIRE(pl != nullptr);
					REQUIRE(pl->variable().fullname() == name1);
					auto pr = dynamic_cast<const mg::unpack_variable_action *>(&op.right().as_expression());
					REQUIRE(pr != nullptr);
					REQUIRE(pr->variable().fullname() == name2);
				};
				check_parsing("(x1) + (x2)", "x_1", "x_2");
				check_parsing("x1+(x2)", "x_1", "x_2");
				check_parsing("(x1)+a2", "x_1", "a_2");
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
			mg::binary_operator_action op{ { 1.0 }, mg::unique_operations::add, { 2.0 } };
			REQUIRE(&op.operation() == &mg::unique_operations::add);
			REQUIRE(op.deps().size() == 0);
			REQUIRE(op.left().is_number());
			REQUIRE(op.right().is_number());
			REQUIRE(op.left().as_number() == 1.0);
			REQUIRE(op.right().as_number() == 2.0);
		}
		SECTION("number and var")
		{
			mg::unpack_variable_action x{ "x" };
			mg::binary_operator_action op{ { 1.0 }, mg::unique_operations::add, x };
			REQUIRE(&op.operation() == &mg::unique_operations::add);
			REQUIRE(op.deps().size() == 1);
			REQUIRE(op.deps().contains({ "x" }));
			REQUIRE(op.left().is_number());
			REQUIRE(op.right().is_expression());
			REQUIRE(op.left().as_number() == 1.0);
			auto p = dynamic_cast<const mg::unpack_variable_action *>(&op.right().as_expression());
			REQUIRE(p != nullptr);
			REQUIRE(p->variable().fullname() == "x");
		}
		SECTION("var and number")
		{
			mg::unpack_variable_action x{ "x" };
			mg::binary_operator_action op{ x, mg::unique_operations::add, { 1.0 } };
			REQUIRE(&op.operation() == &mg::unique_operations::add);
			REQUIRE(op.deps().size() == 1);
			REQUIRE(op.deps().contains({ "x" }));
			REQUIRE(op.left().is_expression());
			auto p = dynamic_cast<const mg::unpack_variable_action *>(&op.left().as_expression());
			REQUIRE(p != nullptr);
			REQUIRE(p->variable().fullname() == "x");
			REQUIRE(op.right().is_number());
			REQUIRE(op.right().as_number() == 1.0);
		}
		SECTION("var and var")
		{
			mg::unpack_variable_action x{ "x" }, x1{ "x1" };
			mg::binary_operator_action op{ x, mg::unique_operations::add, x1 };
			REQUIRE(&op.operation() == &mg::unique_operations::add);
			REQUIRE(op.deps().size() == 2);
			REQUIRE(op.deps().contains({ "x" }));
			REQUIRE(!op.deps().contains({ "x1" })); // SHOULD NOT CONTAIN "x1" CAUSE IT WILL RENAMED TO "x_1"
			REQUIRE(op.deps().contains({ "x_1" }));
			auto pl = dynamic_cast<const mg::unpack_variable_action *>(&op.left().as_expression());
			REQUIRE(pl != nullptr);
			REQUIRE(pl->variable().fullname() == "x");

			auto pr = dynamic_cast<const mg::unpack_variable_action *>(&op.right().as_expression());
			REQUIRE(pr != nullptr);
			REQUIRE(pr->variable().fullname() == "x_1");
		}
		SECTION("with difficult expressions")
		{
			SECTION("number and other operation")
			{
				mg::binary_operator_action op1{ "x + 2" };
				mg::binary_operator_action op{ { 1.0 }, mg::unique_operations::add, op1 };
				REQUIRE(&op.operation() == &mg::unique_operations::add);
				REQUIRE(op.deps().size() == 1);
				REQUIRE(op.deps().contains({ "x" }));
				REQUIRE(op.left().is_number());
				REQUIRE(op.right().is_expression());
			}
			SECTION("var and other operation")
			{
				auto test = [](mg::string_type opact_string, mg::string_type var_name, size_t n) {
					mg::binary_operator_action op1{ opact_string };
					mg::unpack_variable_action x{ var_name };
					mg::binary_operator_action op{ x, mg::unique_operations::add, op1 };
					REQUIRE(&op.operation() == &mg::unique_operations::add);
					REQUIRE(op.deps().size() == n);
					return op;
				};
				SECTION("with same vars")
				{
					auto op = test("x + 2", "x", 1);
					REQUIRE(op.deps().contains({ "x" }));
				}
				SECTION("with different vars")
				{
					auto op = test("x + 2", "y", 2);
					REQUIRE(op.deps().contains({ "x" }));
					REQUIRE(op.deps().contains({ "y" }));
				}
				SECTION("with many different vars")
				{
					auto op = test("x + z", "y", 3);
					REQUIRE(op.deps().size() == 3);
					REQUIRE(op.deps().contains({ "x" }));
					REQUIRE(op.deps().contains({ "y" }));
					REQUIRE(op.deps().contains({ "z" }));
				}
			}
			SECTION("operation and other operation")
			{
				auto test = [](mg::string_type opact_string, mg::string_type other_opact_string, size_t n) {
					mg::binary_operator_action op1{ opact_string };
					mg::binary_operator_action op2{ other_opact_string };
					mg::binary_operator_action op{ op1, mg::unique_operations::add, op2 };
					REQUIRE(&op.operation() == &mg::unique_operations::add);
					REQUIRE(op.deps().size() == n);
					REQUIRE(op.left().is_expression());
					REQUIRE(op.right().is_expression());
					return op;
				};
				SECTION("with same vars")
				{
					auto op = test("x + x", "x + 3", 1);
					REQUIRE(op.deps().contains({ "x" }));
				}
				SECTION("with different vars")
				{
					auto op = test("x + 2", "y + 3", 2);
					REQUIRE(op.deps().contains({ "x" }));
					REQUIRE(op.deps().contains({ "y" }));
				}
				SECTION("with many different vars")
				{
					auto op = test("x + z", "y + w", 4);
					REQUIRE(op.deps().contains({ "x" }));
					REQUIRE(op.deps().contains({ "y" }));
					REQUIRE(op.deps().contains({ "z" }));
					REQUIRE(op.deps().contains({ "w" }));
				}
			}
		}
	}
}
