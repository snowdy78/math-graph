#include <iostream>
#include "operator_action.hpp"
#include "variable.hpp"

using namespace std::string_literals;
void print_var(const mg::variable &x)
{
	if (x.number_ref())
	{
		std::cout << "'" << x.as_number() << "'\n";
	}
	else if (x.independent())
	{
		std::cout << "'" << x.name() << "'\n";
	}
	else
	{
		std::cout << "'" << x.as_variable().name() << "'\n";
	}
}
int main()
{
	mg::number n{ "3.14" };
	mg::variable y{ "y" };
	mg::variable x{ "x", y };
	mg::variable z{ "z", n };
	mg::operator_action pa("2 + x");
	auto &final_refer = mg::find_final_variable(x);
	print_var(final_refer);
	print_var(x);
	print_var(y);
	print_var(z);
	return 0;
}
