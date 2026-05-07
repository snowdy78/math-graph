
#include <catch2/catch_test_macros.hpp>
#include "math_graph/variable_declaration.hpp"

TEST_CASE("variable_declaration construct", "[test]")
{
	SECTION("construct by name")
	{
		SECTION("simple name")
		{
			mg::variable_declaration x{ "x" };
			REQUIRE(x.name() == "x");
			REQUIRE(x.numerator() == "");
			REQUIRE(x.fullname() == "x");
		}
		SECTION("name with numeration")
		{
			mg::variable_declaration x1{ "x12" };
			REQUIRE(x1.name() == "x");
			REQUIRE(x1.numerator() == "12");
			REQUIRE(x1.fullname() == "x_12");
		}
		SECTION("name with string numerator")
		{
			mg::variable_declaration x{ "x_a1" };
			REQUIRE(x.fullname() == "x_a1");
			REQUIRE(x.numerator() == "a1");
			REQUIRE(x.name() == "x");
		}
		SECTION("name with numerator")
		{
			mg::variable_declaration x{ "x_1" };
			REQUIRE(x.fullname() == "x_1");
			REQUIRE(x.numerator() == "1");
			REQUIRE(x.name() == "x");
		}
	}
	SECTION("parse error")
	{
		REQUIRE_THROWS(mg::variable_declaration{ "1" });
		REQUIRE_THROWS(mg::variable_declaration{ "1a" });
		REQUIRE_THROWS(mg::variable_declaration{ "_x" });
		REQUIRE_THROWS(mg::variable_declaration{ "x1_" });
		REQUIRE_THROWS(mg::variable_declaration{ "x1_a" });
		REQUIRE_THROWS(mg::variable_declaration{ "xa" });
		REQUIRE_THROWS(mg::variable_declaration{ "x1_1" });
	}
}
