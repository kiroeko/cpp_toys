#include "encode/utf8/u8char.h"
#include "encode/utf8/u8string.h"
#include <string>

using namespace liquid::encode;

int main()
{
	u8char c0('a');
	u8char c1(u8'b');
	u8char c2(15118729ul);
	u8char c3(u8"码");
	auto c4 = "云"_utf8char;

	u8string str0 = u8"你好，世界！";
	u8string str1(u8"编码");
	u8string str2("开发");
	auto str3 = "成员函数"_utf8str;

	str0 += c2;
	std::cout << str0 << std::endl;

	for (auto& c : str1)
	{
		std::cout << c << std::endl;
	}

	return 0;
}