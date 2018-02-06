#include <iostream>
#include "IException.hpp"

void foo3()
{
	throw IException("Test exception");
}

void foo2()
{
	foo3();
}

void foo1()
{
	foo2();
}

int main()
{
	try
	{
		foo1();
	}
	catch(std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}