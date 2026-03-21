#include <catch2/catch_test_macros.hpp>

#include "number.hpp"

TEST_CASE("construct number", "[.number]")
{
	SECTION("construct by number")
	{
		mg::number n{ 2.0 };
		REQUIRE(n == 2.0);
	}
	SECTION("construct by string")
	{
		mg::number n{ "2.0" };
		REQUIRE(n == 2.0);
		n = { "2" };
		REQUIRE(n == 2.0);
		n = { "2." };
		REQUIRE(n == 2.0);
		n = { ".0" };
		REQUIRE(n == 0.0);
	}
	SECTION("error construction")
	{
		REQUIRE_THROWS(mg::number{ "." }); // does not to be a 0.0, should throw an exception!
		REQUIRE_THROWS(mg::number{ "_2.0" });
		REQUIRE_THROWS(mg::number{ "a" });
	}
}
TEST_CASE("operations with number", "[.number]")
{
	SECTION("add")
	{
		mg::number n1{ "2.0" };
		mg::number n2{ "2.0" };
		REQUIRE(n1 + n2 == 4.0);
	}
	SECTION("mult")
	{
		mg::number n1{ "2.0" };
		mg::number n2{ "2.0" };
		REQUIRE(n1 * n2 == 4.0);
	}
	SECTION("divide")
	{
		mg::number n1{ "2.0" };
		mg::number n2{ "2.0" };
		REQUIRE(n1 / n2 == 1.0);
	}
	SECTION("substract")
	{
		mg::number n1{ "2.0" };
		mg::number n2{ "2.0" };
		REQUIRE(n1 - n2 == 0.0);
	}
}
