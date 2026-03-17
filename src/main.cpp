#include <iostream>
#include "operation.hpp"
#include "operator_action.hpp"
#include "parameter.hpp"

using namespace std::string_literals;

int main()
{
	mg::number n{ "3.14" };
	mg::variable v{ "x" };
	mg::parameter p("p", { n });
	mg::operator_action pa("2 + 2");
	std::cout << "'" << std::get<mg::number>(pa.compute()) << "'\n";
	return 0;
}
