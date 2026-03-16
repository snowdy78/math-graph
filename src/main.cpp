#include <iostream>
#include "action.hpp"

using namespace std::string_literals;

int main()
{
	mg::number n{ "3.14" };
	mg::variable v{ "x" };
	mg::parameter p("p", n);
	std::cout << p.as_number() << "\n";
	return 0;
}
