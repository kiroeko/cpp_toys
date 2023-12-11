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

			u8char(const char8_t* const utf8_char);
			u8char(const std::u8string& utf8_char) : u8char(utf8_char.c_str()) {}
			u8char(const std::vector<char8_t>& utf8_char);

			u8char(const char* const utf8_char);
			u8char(const std::string& utf8_char) : u8char(utf8_char.c_str()) {}
			u8char(const std::vector<char>& utf8_char);

			bool operator<(const u8char& other) const {
				return _encode < other._encode;
			}

			bool operator>(const u8char& other) const {
				return _encode > other._encode;
			}

			bool operator==(const u8char& other) const {
				return _encode == other._encode;
			}

			bool operator>=(const u8char& other) const {
				return _encode >= other._encode;
			}

			bool operator<=(const u8char& other) const {
				return _encode <= other._encode;
			}

			bool operator!=(const u8char& other) const {
				return _encode != other._encode;
			}

			friend std::ostream& operator<<(std::ostream& os, const u8char& utf8_char) {
				if (!utf8_char.is_vaild_encode())
					return os;
				
				return os << utf8_char._string_representation;
			}

			friend std::istream& operator>>(std::istream& is, u8char& utf8_char) {
				std::string tmp;
				is >> tmp;

				u8char c(tmp);
				utf8_char = c;

				return is;
			}

			operator std::string() const {
				return _string_representation;
			}

			operator std::u8string() const {
				return _u8string_representation;
			}

			unsigned long get_encode() const {
				return _encode;
			}

			std::string to_string() const {
				return _string_representation;
			}

			std::u8string to_u8string() const {
				return _u8string_representation;
			}

			size_t part_size() const {
				return _part_count;
			}

			char8_t get_part(size_t index) const {
				return _parts[index];
			}
		private:
			unsigned short generate_bytes_from_encode();
			void generate_encode_from_bytes(unsigned short byte_count);

			void generate_strings() {
				_string_representation = generate_string();
				_u8string_representation = generate_u8string();
			}

			std::string generate_string() const {
				std::string str;
				for (int i = 0; i < _part_count; ++i)
				{
					str.push_back(_parts[i]);
				}
				return str;
			}

			std::u8string generate_u8string() const {
				std::u8string str;
				for (int i = 0; i < _part_count; ++i)
				{
					str.push_back(_parts[i]);
				}
				return str;
			}

			bool is_vaild_encode() const {
				if (_part_count == 0)
					return false;
				if (0 <= _encode && _encode <= 0xEFBFBFBF)
					return true;
				return false;
			}

			inline void clear();

			char8_t _parts[4] = { 0, 0, 0, 0 };
			unsigned short _part_count = 0;
			unsigned long _encode = 0;
			std::string _string_representation = "";
			std::u8string _u8string_representation = u8"";
		};
	}
}

#endif