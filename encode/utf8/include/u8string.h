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
			u8string(const std::u8string& string);
			u8string(const char8_t* const string) : u8string((std::u8string)string) {}
			u8string(const std::vector<char8_t>& string) : u8string(string.data()) {}

			u8string(const std::string& string);
			u8string(const char* const string) : u8string((std::string)string) {}
			u8string(const std::vector<char>& string) : u8string(string.data()) {}

			u8string(const std::vector<u8char>& string);

			std::vector<u8string> split(const u8char& delimiter);

			u8char operator[](size_t index) const {
				return _string[index];
			}

			bool operator==(const u8char& other) const {
				return to_string() == other.to_string();
			}

			bool operator!=(const u8char& other) const {
				return to_string() != other.to_string();
			}

			friend u8string operator+(const u8string& u8str1, const u8string& u8str2) {
				std::vector<u8char> tmp = u8str1._string;
				for (const auto& c : u8str2._string) {
					tmp.push_back(c);
				}
				u8string tmp_str(tmp);
				return tmp;
			}

			friend u8string operator+=(u8string& u8str, const u8char& utf8_char) {
				u8str._string.push_back(utf8_char);
				return u8str;
			}

			friend u8string operator+=(u8string& u8str, const u8string& u8str2) {
				u8str._string.append_range(u8str2._string);
				return u8str;
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

			auto begin() {
				return _string.begin();
			}

			auto end() {
				return _string.end();
			}

			auto rbegin() {
				return _string.rbegin();
			}

			auto rend() {
				return _string.rend();
			}

			auto cbegin() const {
				return _string.cbegin();
			}

			auto cend() const {
				return _string.cend();
			}

			auto crbegin() const {
				return _string.crbegin();
			}

			auto crend() const {
				return _string.crend();
			}

			operator std::string() const {
				return to_string();
			}

			operator std::u8string() const {
				return to_u8string();
			}

			bool empty() const {
				return _string.size() == 0;
			}

			size_t size() const {
				return _string.size();
			}

			std::string to_string() const;

			std::u8string to_u8string() const;
		private:
			void clear()
			{
				_string.clear();
			}

			std::vector<u8char> _string;
		};
	}
}

#endif