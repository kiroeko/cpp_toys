#ifndef __UTF8CHAR_H__
#define __UTF8CHAR_H__

#if __cplusplus >= 201700L && __cplusplus < 202000L
// Implement for the version C++17.

#include <iostream>
#include <string>
#include "encode/utf8/u8exception.h"
#include <vector>

namespace liquid::encode
{
	class u8char
	{
	public:
		u8char(const unsigned long& utf8_encode)
		{
			_encode = utf8_encode;
			_part_count = generate_bytes_from_encode();
			generate_strings();
		}

		u8char(const unsigned char& utf8_char)
		{
			_encode = utf8_char;
			generate_bytes_from_encode();
			_part_count = 1;
			generate_strings();
		}

		u8char(const char& utf8_char) : u8char((unsigned char)utf8_char) {}

		u8char(const char* const utf8_char)
		{
			bool good_end = false;
			for (int i = 0; i < 5; ++i)
			{
				char c = *(utf8_char + i);
				if (c == '\0')
				{
					good_end = true;
					_part_count = i + 1;
					break;
				}
				if (i < 4)
				{
					_parts[i] = c;
				}
			}

			if (!good_end)
			{
				throw u8exception (
					U8_EXCEPTION_TYPE::INVAILD_UTF8_ENCODE,
					"The char array passed as a utf8 char needs to be within 4 bytes and end with '\\0'."
				);
			}

			generate_encode_from_bytes(_part_count);
			generate_strings();
		}

		u8char(const std::string& utf8_char) : u8char(utf8_char.c_str()) {}

		u8char(const std::vector<char>& utf8_char)
		{
			int s = utf8_char.size();
			if (s > 4)
			{
				throw u8exception (
					U8_EXCEPTION_TYPE::INVAILD_UTF8_ENCODE,
					"A utf-8 character needs to be within 4 bytes."
				);
			}

			for (int i = 0; i < s; ++i)
			{
				_parts[i] = utf8_char[i];
			}
			_part_count = s;
			generate_encode_from_bytes(_part_count);
			generate_strings();
		}

		bool operator<(const u8char& other) const
		{
			return _encode < other._encode;
		}

		bool operator>(const u8char& other) const
		{
			return _encode > other._encode;
		}

		bool operator==(const u8char& other) const
		{
			return _encode == other._encode;
		}

		bool operator>=(const u8char& other) const
		{
			return _encode >= other._encode;
		}

		bool operator<=(const u8char& other) const
		{
			return _encode <= other._encode;
		}

		bool operator!=(const u8char& other) const
		{
			return _encode != other._encode;
		}

		friend std::ostream& operator<<(std::ostream& os, const u8char& utf8_char)
		{
			return os << utf8_char._string_representation;
		}

		friend std::istream& operator>>(std::istream& is, u8char& utf8_char)
		{
			std::string tmp;
			is >> tmp;

			u8char c(tmp);
			utf8_char = c;

			return is;
		}

		operator std::string() const
		{
			return _string_representation;
		}

		unsigned long get_encode() const
		{
			return _encode;
		}

		std::string to_string() const
		{
			return _string_representation;
		}

		size_t part_size() const
		{
			return _part_count;
		}

		unsigned char get_part(size_t index) const
		{
			return _parts[index];
		}
	private:
		unsigned short generate_bytes_from_encode()
		{
			if (!is_vaild_encode())
			{
				clear();
				throw u8exception (
					U8_EXCEPTION_TYPE::INVAILD_UTF8_ENCODE,
					"The character range of utf-8 needs to be between 0 and 0xEFBFBFBF."
				);
			}

			bool partHasValue = false;
			unsigned short part_count = 0;

			int partIdx = 0;
			for (int i = 3; i >= 0; --i)
			{
				unsigned char c = (unsigned char)(_encode >> (8 * i)) & (unsigned char)0xFF;

				if (!partHasValue)
				{
					if (c != 0)
					{
						partHasValue = true;
						part_count = i + 1;
					}
					else
						continue;
				}

				_parts[partIdx] = c;
				++partIdx;
			}

			return part_count;
		}

		void generate_encode_from_bytes(unsigned short byte_count)
		{
			_encode = 0;
			for (int i = 0; i < byte_count; ++i)
			{
				_encode |= _parts[i] << (8 * (4 - 1 - i));
			}

			if (!is_vaild_encode())
			{
				clear();
				throw u8exception (
					U8_EXCEPTION_TYPE::INVAILD_UTF8_ENCODE,
					"The character range of utf-8 needs to be between 0 and 0xEFBFBFBF."
				);
			}
		}

		void generate_strings()
		{
			_string_representation = generate_string();
		}

		std::string generate_string() const
		{
			std::string str;
			for (int i = 0; i < _part_count; ++i)
			{
				str.push_back(_parts[i]);
			}
			return str;
		}

		bool is_vaild_encode() const
		{
			if (0 <= _encode && _encode <= 0xEFBFBFBF)
				return true;
			return false;
		}

		inline void clear()
		{
			for (unsigned char& b : _parts)
			{
				b = 0;
			}
			_part_count = 0;
			_encode = 0;

			_string_representation.clear();
		}

		unsigned char _parts[4] = { 0, 0, 0, 0 };
		unsigned short _part_count = 0;
		unsigned long _encode = 0;
		std::string _string_representation = "";
	};
}
#else
// Implement for the version C++20 and later.

#include <iostream>
#include <string>
#include "u8exception.h"
#include <vector>

namespace liquid::encode
{
	class u8char
	{
	public:
		u8char(const unsigned long& utf8_encode)
		{
			_encode = utf8_encode;
			_part_count = generate_bytes_from_encode();
			generate_strings();
		}

		u8char(const char8_t& utf8_char)
		{
			_encode = utf8_char;
			generate_bytes_from_encode();
			_part_count = 1;
			generate_strings();
		}

		u8char(const unsigned char& utf8_char)
		{
			_encode = utf8_char;
			generate_bytes_from_encode();
			_part_count = 1;
			generate_strings();
		}

		u8char(const char& utf8_char) : u8char((unsigned char)utf8_char) {}

		u8char(const char8_t* const utf8_char)
		{
			bool good_end = false;
			for (int i = 0; i < 5; ++i)
			{
				char8_t c = *(utf8_char + i);
				if (c == '\0')
				{
					good_end = true;
					_part_count = i + 1;
					break;
				}
				if (i < 4)
				{
					_parts[i] = c;
				}
			}

			if (!good_end)
			{
				throw u8exception (
					U8_EXCEPTION_TYPE::INVAILD_UTF8_ENCODE,
					"The char array passed as a utf8 char needs to be within 4 bytes and end with '\\0'."
				);
			}

			generate_encode_from_bytes(_part_count);
			generate_strings();
		}

		u8char(const std::u8string& utf8_char) : u8char(utf8_char.c_str()) {}

		u8char(const std::vector<char8_t>& utf8_char)
		{
			int s = utf8_char.size();
			if (s > 4)
			{
				throw u8exception (
					U8_EXCEPTION_TYPE::INVAILD_UTF8_ENCODE,
					"A utf-8 character needs to be within 4 bytes."
				);
			}

			for (int i = 0; i < s; ++i)
			{
				_parts[i] = utf8_char[i];
			}
			_part_count = s;
			generate_encode_from_bytes(_part_count);
			generate_strings();
		}

		u8char(const char* const utf8_char)
		{
			bool good_end = false;
			for (int i = 0; i < 5; ++i)
			{
				char c = *(utf8_char + i);
				if (c == '\0')
				{
					good_end = true;
					_part_count = i + 1;
					break;
				}
				if (i < 4)
				{
					_parts[i] = c;
				}
			}

			if (!good_end)
			{
				throw u8exception (
					U8_EXCEPTION_TYPE::INVAILD_UTF8_ENCODE,
					"The char array passed as a utf8 char needs to be within 4 bytes and end with '\\0'."
				);
			}

			generate_encode_from_bytes(_part_count);
			generate_strings();
		}

		u8char(const std::string& utf8_char) : u8char(utf8_char.c_str()) {}

		u8char(const std::vector<char>& utf8_char)
		{
			int s = utf8_char.size();
			if (s > 4)
			{
				throw u8exception (
					U8_EXCEPTION_TYPE::INVAILD_UTF8_ENCODE,
					"A utf-8 character needs to be within 4 bytes."
				);
			}

			for (int i = 0; i < s; ++i)
			{
				_parts[i] = utf8_char[i];
			}
			_part_count = s;
			generate_encode_from_bytes(_part_count);
			generate_strings();
		}

		bool operator<(const u8char& other) const
		{
			return _encode < other._encode;
		}

		bool operator>(const u8char& other) const
		{
			return _encode > other._encode;
		}

		bool operator==(const u8char& other) const
		{
			return _encode == other._encode;
		}

		bool operator>=(const u8char& other) const
		{
			return _encode >= other._encode;
		}

		bool operator<=(const u8char& other) const
		{
			return _encode <= other._encode;
		}

		bool operator!=(const u8char& other) const
		{
			return _encode != other._encode;
		}

		friend std::ostream& operator<<(std::ostream& os, const u8char& utf8_char)
		{
			return os << utf8_char._string_representation;
		}

		friend std::istream& operator>>(std::istream& is, u8char& utf8_char)
		{
			std::string tmp;
			is >> tmp;

			u8char c(tmp);
			utf8_char = c;

			return is;
		}

		operator std::string() const
		{
			return _string_representation;
		}

		operator std::u8string() const
		{
			return _u8string_representation;
		}

		unsigned long get_encode() const
		{
			return _encode;
		}

		std::string to_string() const
		{
			return _string_representation;
		}

		std::u8string to_u8string() const
		{
			return _u8string_representation;
		}

		size_t part_size() const
		{
			return _part_count;
		}

		char8_t get_part(size_t index) const
		{
			return _parts[index];
		}
	private:
		unsigned short generate_bytes_from_encode()
		{
			if (!is_vaild_encode())
			{
				clear();
				throw u8exception (
					U8_EXCEPTION_TYPE::INVAILD_UTF8_ENCODE,
					"The character range of utf-8 needs to be between 0 and 0xEFBFBFBF."
				);
			}

			bool partHasValue = false;
			unsigned short part_count = 0;

			int partIdx = 0;
			for (int i = 3; i >= 0; --i)
			{
				char8_t c = (char8_t)(_encode >> (8 * i)) & (char8_t)0xFF;

				if (!partHasValue)
				{
					if (c != 0)
					{
						partHasValue = true;
						part_count = i + 1;
					}
					else
						continue;
				}

				_parts[partIdx] = c;
				++partIdx;
			}

			return part_count;
		}

		void generate_encode_from_bytes(unsigned short byte_count)
		{
			_encode = 0;
			for (int i = 0; i < byte_count; ++i)
			{
				_encode |= _parts[i] << (8 * (4 - 1 - i));
			}

			if (!is_vaild_encode())
			{
				clear();
				throw u8exception (
					U8_EXCEPTION_TYPE::INVAILD_UTF8_ENCODE,
					"The character range of utf-8 needs to be between 0 and 0xEFBFBFBF."
				);
			}
		}

		void generate_strings()
		{
			_string_representation = generate_string();
			_u8string_representation = generate_u8string();
		}

		std::string generate_string() const
		{
			std::string str;
			for (int i = 0; i < _part_count; ++i)
			{
				str.push_back(_parts[i]);
			}
			return str;
		}

		std::u8string generate_u8string() const
		{
			std::u8string str;
			for (int i = 0; i < _part_count; ++i)
			{
				str.push_back(_parts[i]);
			}
			return str;
		}

		bool is_vaild_encode() const
		{
			if (0 <= _encode && _encode <= 0xEFBFBFBF)
				return true;
			return false;
		}

		inline void clear()
		{
			for (char8_t& b : _parts)
			{
				b = 0;
			}
			_part_count = 0;
			_encode = 0;

			_string_representation.clear();
			_u8string_representation.clear();
		}

		char8_t _parts[4] = { 0, 0, 0, 0 };
		unsigned short _part_count = 0;
		unsigned long _encode = 0;
		std::string _string_representation = "";
		std::u8string _u8string_representation = u8"";
	};
}
#endif

namespace liquid::encode
{
	u8char operator""_utf8char(const char* str, std::size_t size)
	{
		return u8char(std::string(str, size));
	}
}

#endif