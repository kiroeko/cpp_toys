#include "u8char.h"
#include "u8string.h"
#include <string>

int main()
{
	liquid::encode::u8char c0('a');
	liquid::encode::u8char c1(u8'b');
	liquid::encode::u8char c2(15118729ul);
	liquid::encode::u8char c3(u8"码");

	std::u8string tmps0 = u8"你好，世界！";
	liquid::encode::u8string str0 = tmps0;
	liquid::encode::u8string str1(u8"编码");

	str0 += c2;
	std::cout << str0 << std::endl;

	for (auto& c : str1)
	{
		std::cout << c << std::endl;
	}

	return 0;
}