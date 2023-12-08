#ifndef __U8STRING_H__
#define __U8STRING_H__

#include <iostream>
#include <vector>
#include <string>
#include "u8char.h"

namespace liquid {
	namespace encode {
		class u8string
		{
		public:
			u8string(const std::string& string);
			u8string(const char* const utf8_char) : u8string((std::string)utf8_char) {}

			u8string(const std::u8string& utf8_string);
			u8string(const char8_t* const utf8_char) : u8string((std::u8string)utf8_char) {}

			u8char operator[](size_t index) const {
				return _string[index];
			}

			friend std::ostream& operator<<(std::ostream& os, const u8string& utf8_string) {
				for (const auto& c : utf8_string._string)
				{
					os << c;
				}
				return os;
			}

			friend std::istream& operator>>(std::istream& is, u8string& utf8_string) {
				std::string tmp;
				is >> tmp;

				u8string s(tmp);
				utf8_string = s;

				return is;
			}

			operator std::string() const {
				return to_string();
			}

			operator std::u8string() const {
				return to_u8string();
			}

			bool empty()
			{
				return _string.size() == 0;
			}

			size_t size() const {
				return _string.size();
			}

			std::string to_string() const;

			std::u8string to_u8string() const;
		private:
			inline void clear()
			{
				_string.clear();
			}

			std::vector<u8char> _string;
		};
	}
}

#endif