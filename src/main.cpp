#include <iostream>
#include <variant>
#include "independent_variable.hpp"
#include "action.hpp"
#include "expression.hpp"
#include "tools.hpp"

int main()
{
	mg::number n{ "3.14" };
	mg::independent_variable x{ "x" };
	mg::operator_action pa("2 + x");
	auto result = mg::get_result(
		{
			pa
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
