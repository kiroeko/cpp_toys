#include "u8char.h"
#include "u8string.h"
#include <string>

int main()
{
	liquid::encode::u8string str0(u8"你好世");
	liquid::encode::u8char c0(u8"界");

	auto str1 = str0 += c0;
	std::cout << str1 << std::endl;

	return 0;
}