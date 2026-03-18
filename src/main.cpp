#include <iostream>
#include "operator_action.hpp"
#include "parameter.hpp"
#include "unresolved_expression.hpp"

using namespace std::string_literals;

int main()
{
	mg::number n{ "3.14" };
	mg::variable v{ "x" };
	mg::parameter p("p", { n });
	mg::operator_action pa("2 + x");
	std::cout << "'" << std::get<mg::unresolved_expression>(pa.compute()).str(" ") << "'\n";
	return 0;
}
