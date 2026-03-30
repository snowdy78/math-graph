
#include <catch2/catch_test_macros.hpp>

#include "math_graph/operation.hpp"

TEST_CASE("operation use", "[test]")
{
	SECTION("test binary operations")
	{
		SECTION("usage")
		{
			auto add = mg::unique_operations::add(1, 2);
			CHECK(add == 3);
			auto mult = mg::unique_operations::mul(2, 2);
			CHECK(mult == 4);
			auto div = mg::unique_operations::div(4, 2);
			CHECK(div == 2);
			auto pow = mg::unique_operations::pow(2, 3);
			CHECK(pow == 8);
			auto sub = mg::unique_operations::sub(2, 2);
			CHECK(sub == 0);
		}
		SECTION("get by name")
		{
			auto &add = mg::binary_operation::get_by_name('+');
			CHECK(&add == &mg::unique_operations::add);
			auto &sub = mg::binary_operation::get_by_name('-');
			CHECK(&sub == &mg::unique_operations::sub);
			auto &mult = mg::binary_operation::get_by_name('*');
			CHECK(&mult == &mg::unique_operations::mul);
			auto &div = mg::binary_operation::get_by_name('/');
			CHECK(&div == &mg::unique_operations::div);
			auto &pow = mg::binary_operation::get_by_name('^');
			CHECK(&pow == &mg::unique_operations::pow);
		}
	}
	SECTION("test unary operations")
	{
		SECTION("usage")
		{
			auto plus = mg::unique_operations::plus(2);
			CHECK(plus == 2);
			auto minus = mg::unique_operations::minus(4);
			CHECK(minus == -4);
		}
		SECTION("get by name")
		{
			auto &plus = mg::unary_operation::get_by_name('+');
			CHECK(&plus == &mg::unique_operations::plus);
			auto &minus = mg::unary_operation::get_by_name('-');
			CHECK(&minus == &mg::unique_operations::minus);
		}
	}
}
