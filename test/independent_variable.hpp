
#include <catch2/catch_test_macros.hpp>
#include "math_graph/independent_variable.hpp"

TEST_CASE("independent_variable construct", "[test]")
{
	SECTION("construct by name")
	{
		SECTION("simple name")
		{
			mg::independent_variable x{ "x" };
			REQUIRE(x.name() == "x");
			REQUIRE(x.numerator() == "");
			REQUIRE(x.fullname() == "x");
		}
		SECTION("name with numeration")
		{
			mg::independent_variable x1{ "x12" };
			REQUIRE(x1.name() == "x");
			REQUIRE(x1.numerator() == "12");
			REQUIRE(x1.fullname() == "x_12");
		}
		SECTION("name with string numerator")
		{
			mg::independent_variable x{ "x_a1" };
			REQUIRE(x.fullname() == "x_a1");
			REQUIRE(x.numerator() == "a1");
			REQUIRE(x.name() == "x");
		}
		SECTION("name with numerator")
		{
			mg::independent_variable x{ "x_1" };
			REQUIRE(x.fullname() == "x_1");
			REQUIRE(x.numerator() == "1");
			REQUIRE(x.name() == "x");
		}
	}
	SECTION("parse error")
	{
		REQUIRE_THROWS(mg::independent_variable{ "1" });
		REQUIRE_THROWS(mg::independent_variable{ "1a" });
		REQUIRE_THROWS(mg::independent_variable{ "_x" });
		REQUIRE_THROWS(mg::independent_variable{ "x1_" });
		REQUIRE_THROWS(mg::independent_variable{ "x1_a" });
		REQUIRE_THROWS(mg::independent_variable{ "xa" });
		REQUIRE_THROWS(mg::independent_variable{ "x1_1" });
	}
}
