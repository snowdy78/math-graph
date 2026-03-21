#include "catch2/catch_test_macros.hpp"
#include "math_graph/action.hpp"

TEST_CASE("action construct", "[test]")
{
	SECTION("construct by operator_action")
	{
		mg::operator_action op{ "x + y" };
		mg::action act{ op };
		REQUIRE(act.is_operator_action() == true);
		REQUIRE(act.deps().size() == 2);
		REQUIRE(act.deps().contains({ "x" }));
		REQUIRE(act.deps().contains({ "y" }));
		REQUIRE_NOTHROW(act.as_operation());
		REQUIRE_THROWS(act.as_function());
	}
	SECTION("construct by function")
	{
		mg::function op{ "f(x)", [](const auto &x) -> mg::function::return_type {
							return x.at({ "x" });
						} };
		mg::action act{ op };
		REQUIRE(act.is_function() == true);
		REQUIRE(act.deps().size() == 1);
		REQUIRE(act.deps().contains({ "x" }));
		REQUIRE_NOTHROW(act.as_function());
		REQUIRE_THROWS(act.as_operation());
	}
}
