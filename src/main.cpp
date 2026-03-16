#include <iostream>
#include "operator_action.hpp"
#include "parameter.hpp"

using namespace std::string_literals;

int main()
{
	mg::number n{ "3.14" };
	mg::variable v{ "x" };
	mg::parameter p("p", { n });
	mg::operation add("+");
	mg::operator_action pa(p, add, { n });
	std::cout << "'" << pa.left().as_number() << "'\n";
	return 0;
}
