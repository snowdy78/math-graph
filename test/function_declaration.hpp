#include "catch2/catch_test_macros.hpp"
#include "math_graph/function_declaration.hpp"

TEST_CASE("function construct", "[test]")
{
	SECTION("construct by string")
	{
		auto contains_argument = [](const mg::function_declaration &f, mg::string_type name) {
			return std::find_if(
					   f.args().begin(), f.args().end(),
					   [&name](const auto &arg) {
						   return arg.fullname() == name;
					   }
				   )
				   != f.args().end();
		};
		SECTION("correct name and args")
		{
			mg::function_declaration sqrt{ "f1(x)" };
			REQUIRE(sqrt.fullname() == "f_1");
			REQUIRE(sqrt.args().size() == 1);
			REQUIRE(contains_argument(sqrt, "x"));
		}
		SECTION("multiple args")
		{
			mg::function_declaration pow{ "f_a1(x1, x2)" };
			REQUIRE(pow.fullname() == "f_a1");
			REQUIRE(pow.args().size() == 2);
			REQUIRE(contains_argument(pow, "x_1"));
			REQUIRE(contains_argument(pow, "x_2"));
		}
		SECTION("incorrect name")
		{
			REQUIRE_THROWS(mg::function_declaration{ "_f1(x)" });
			REQUIRE_THROWS(mg::function_declaration{ "f1_1(x, x_b, x_c, y)" });
			REQUIRE_THROWS(mg::function_declaration{ "fa(x, x_b, x_c, y)" });
		}
		SECTION("incorrect args")
		{
			SECTION("argument name is function name")
			{
				REQUIRE_THROWS(mg::function_declaration{ "f1(f1)" });
			}
			SECTION("no args")
			{
				REQUIRE_THROWS(mg::function_declaration{ "f1()" });
			}
			SECTION("wrong args")
			{
				REQUIRE_THROWS(mg::function_declaration{ "f1(124121)" });
			}
			SECTION("same args")
			{
				REQUIRE_THROWS(mg::function_declaration{ "f1(x, x)" });
			}
			SECTION("comma without argument")
			{
				REQUIRE_THROWS(mg::function_declaration{ "f1(x, x, )" });
				REQUIRE_THROWS(mg::function_declaration{ "f1(, x, x)" });
			}
			SECTION("argument without comma")
			{
				REQUIRE_THROWS(mg::function_declaration{ "f1(a    c ,b)" });
				REQUIRE_THROWS(mg::function_declaration{ "f1(,,,)" });
			}
		}
	}
}
