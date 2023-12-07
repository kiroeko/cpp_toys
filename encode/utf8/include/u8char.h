#ifndef __UTF8CHAR_H__
#define __UTF8CHAR_H__

#include <vector>
#include <string>

namespace liquid {
	namespace encode {
		class u8char
		{
		public:
			u8char(const char8_t& utf8_char);
			u8char(const unsigned long& utf8_encode);
			u8char(const unsigned char& utf8_char);
			u8char(const wchar_t& utf8_char);
			u8char(const char8_t& byte0, const char8_t& byte1);
			u8char(const char8_t& byte0, const char8_t& byte1, const char8_t& byte2);
			u8char(const char8_t& byte0, const char8_t& byte1, const char8_t& byte2, const char8_t& byte3);
			u8char(const char8_t* const utf8_char, unsigned short byte_count);
			u8char(const std::vector<char8_t>& utf8_char);

			static bool is_vaild_encode(unsigned long utf8_encode) {
				if (0 <= utf8_encode && utf8_encode <= 0x10FFFF)
					return true;
				return false;
			}

			unsigned long get_encode() {
				return _encode;
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

			void clear();

			char8_t _bytes[4];
			unsigned short _byte_count = 0;
			unsigned long _encode = 0;
			std::string _string_representation;
			std::wstring _wstring_representation;
			std::u8string _u8string_representation;
		};
	}
}

#endif