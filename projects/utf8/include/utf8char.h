#ifndef __UTF8CHAR_H__
#define __UTF8CHAR_H__

#include <vector>
#include <string>

namespace liquid {
	namespace encode {
		namespace utf8 {
			class u8char
			{
			public:
				u8char(const char& c);
				u8char(const wchar_t& c);
				u8char(const char8_t& c);
				u8char(const unsigned long& c);
				u8char(const char8_t& c0, const char8_t& c1, const char8_t& c2, const char8_t& c3);
				u8char(const std::vector<char8_t>& c);
				u8char(const std::string& c);
				u8char(const std::wstring& c);
				u8char(const std::u8string& c);
			private:
				char8_t byte0 = 0;
				char8_t byte1 = 0;
				char8_t byte2 = 0;
				char8_t byte3 = 0;
				unsigned long encode = 0;
				std::string str;
				std::wstring wstr;
				std::u8string u8str;
			};

		}
	}
}

#endif