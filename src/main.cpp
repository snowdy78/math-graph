#include <iostream>
#include <variant>
#include "math_graph/action.hpp"
#include "math_graph/expression.hpp"
#include "math_graph/operation.hpp"
#include "math_graph/tools.hpp"

int main()
{
	mg::number n{ "3.14" };
	mg::independent_variable x{ "x" };
	mg::operator_action pa("2 + x");
	mg::operator_action pa2(n, mg::unique_operations::mul, &pa);
	auto result = mg::get_result(
		{
			pa2
	},
		{ { x, n } }
	);
	if (std::holds_alternative<mg::action>(result))
	{
		std::cout << "result is an expression" << std::endl;
	}
	else
	{
		std::cout << std::get<mg::number>(result) << "\n";
	}
	return 0;
}
