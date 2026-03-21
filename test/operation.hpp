
#include <catch2/catch_test_macros.hpp>

#include "math_graph/operation.hpp"

TEST_CASE("operation use", "[test]")
{
	SECTION("use base operations")
	{
		auto add = mg::operation::get_compute_map()[&mg::unique_operations::add](1, 2);
		CHECK(add == 3);
		auto mult = mg::operation::get_compute_map()[&mg::unique_operations::mul](2, 2);
		CHECK(mult == 4);
		auto div = mg::operation::get_compute_map()[&mg::unique_operations::div](4, 2);
		CHECK(div == 2);
		auto pow = mg::operation::get_compute_map()[&mg::unique_operations::pow](2, 2);
		CHECK(pow == 4);
		auto sub = mg::operation::get_compute_map()[&mg::unique_operations::sub](2, 2);
		CHECK(sub == 0);
	}
	SECTION("get operation by name")
	{
		auto &add = mg::operation::get_by_name('+');
		CHECK(add == mg::unique_operations::add);
		auto &mult = mg::operation::get_by_name('*');
		CHECK(mult == mg::unique_operations::mul);
		auto &sub = mg::operation::get_by_name('-');
		CHECK(sub == mg::unique_operations::sub);
		auto &pow = mg::operation::get_by_name('^');
		CHECK(pow == mg::unique_operations::pow);
		auto &div = mg::operation::get_by_name('/');
		CHECK(div == mg::unique_operations::div);
	}
}
