#include <catch2/catch_test_macros.hpp>
#include "independent_variable.hpp"

TEST_CASE("independent_variable construct", "[.independent_variable]")
{
	SECTION("construct by name")
	{
		mg::independent_variable x{ "x" };
		REQUIRE(x.name() == "x");

		mg::independent_variable x1{ "x12" };
		REQUIRE(x1.name() == "x12");
	}
	SECTION("parse error")
	{
		REQUIRE_THROWS(mg::independent_variable{ "1" });
		REQUIRE_THROWS(mg::independent_variable{ "1a" });
		REQUIRE_THROWS(mg::independent_variable{ "_x" });
	}
}
TEST_CASE("independent_variable ", "[.independent_variable]")
{
	mg::independent_variable x{ "x" };
	REQUIRE(x.name() == "x");
}
