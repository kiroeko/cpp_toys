#include "u8char.h"
#include "u8string.h"
#include <string>

int main()
{
	liquid::encode::u8string s = u8"中文";

	std::cout << s << std::endl;

	return 0;
}