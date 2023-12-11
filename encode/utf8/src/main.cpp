#include "u8char.h"
#include "u8string.h"
#include <string>

int main()
{
	liquid::encode::u8char c1(u8"我");
	liquid::encode::u8char c2(u8"我");

	if (c1 == c2)
	{
		std::cout << "Hello, World!" << std::endl;
	}

	return 0;
}