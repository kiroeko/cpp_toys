#ifndef __UTF8CHAR_H__
#define __UTF8CHAR_H__

#include <iostream>
#include <vector>
#include <string>

namespace liquid {
	namespace encode {
		class u8char
		{
		public:
			u8char(const unsigned long& utf8_encode);
			u8char(const char8_t& utf8_char);
			u8char(const unsigned char& utf8_char);
			u8char(const wchar_t& utf8_char);

			u8char(const std::vector<char8_t>& utf8_char);

			u8char(const char8_t* const utf8_char);
			u8char(const std::u8string& utf8_char) : u8char(utf8_char.c_str()) {}

			u8char(const char* const utf8_char);
			u8char(const std::string& utf8_char) : u8char(utf8_char.c_str()) {}

			std::ostream& operator<<(std::ostream& os) const {
				return os << _string_representation;
			}

			std::istream& operator>>(std::istream& is);

			operator std::string() const {
				return _string_representation;
			}

			operator std::wstring() const {
				return _wstring_representation;
			}

			operator std::u8string() const {
				return _u8string_representation;
			}

			unsigned long get_encode() {
				return _encode;
			}

			std::string to_string() {
				return _string_representation;
			}

			std::wstring to_wstring() {
				return _wstring_representation;
			}

			std::u8string to_u8string() {
				return _u8string_representation;
			}
		private:
			unsigned short generate_bytes_from_encode();
			void generate_encode_from_bytes(unsigned short byte_count);

			inline void generate_strings() {
				_string_representation = generate_string();
				_wstring_representation = generate_wstring();
				_u8string_representation = generate_u8string();
			}
			std::string generate_string() {
				std::string str;
				for (int i = 0; i < _byte_count; ++i)
				{
					str.push_back(_bytes[i]);
				}
				return str;
			}
			inline std::wstring generate_wstring() {
				std::wstring str;
				for (int i = 0; i < _byte_count; ++i)
				{
					str.push_back(_bytes[i]);
				}
				return str;
			}
			inline std::u8string generate_u8string() {
				std::u8string str;
				for (int i = 0; i < _byte_count; ++i)
				{
					str.push_back(_bytes[i]);
				}
				return str;
			}

			bool is_vaild_encode() {
				if (_byte_count == 0)
					return false;
				if (0 <= _encode && _encode <= 0x10FFFF)
					return true;
				return false;
			}

			inline void clear();

			char8_t _bytes[4] = { 0, 0, 0, 0 };
			unsigned short _byte_count = 0;
			unsigned long _encode = 0;
			std::string _string_representation = "";
			std::wstring _wstring_representation = L"";
			std::u8string _u8string_representation = u8"";
		};
	}
}

#endif