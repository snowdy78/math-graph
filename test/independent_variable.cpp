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
		try
		{

			mg::independent_variable x{ "1" };
			REQUIRE(false);
		}
		catch (std::exception &err)
		{
			REQUIRE(true);
		}
		try
		{
			mg::independent_variable x{ "1a" };
			REQUIRE(false);
		}
		catch (std::exception &err)
		{
			REQUIRE(true);
		}
		try
		{
			mg::independent_variable x{ "_x" };
			REQUIRE(false);
		}
		catch (std::exception &err)
		{
			REQUIRE(true);
		}
	}
}
TEST_CASE("independent_variable ", "[.independent_variable]")
{
	mg::independent_variable x{ "x" };
	REQUIRE(x.name() == "x");
}
