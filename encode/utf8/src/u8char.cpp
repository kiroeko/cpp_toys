#include "u8char.h"
#include "u8exception.h"

using namespace liquid::encode;

u8char::u8char(const unsigned long& utf8_encode) {
	_encode = utf8_encode;
	_part_count = generate_bytes_from_encode();
	generate_strings();
}

u8char::u8char(const char8_t& utf8_char) {
	_encode = utf8_char;
	generate_bytes_from_encode();
	_part_count = 1;
	generate_strings();
}

u8char::u8char(const unsigned char& utf8_char) {
	_encode = utf8_char;
	generate_bytes_from_encode();
	_part_count = 1;
	generate_strings();
}

u8char::u8char(const char8_t* const utf8_char)
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
		throw u8exception(
			U8_EXCEPTION_TYPE::INVAILD_UTF8_ENCODE,
			"The char array passed as a utf8 char needs to be within 4 bytes and end with '\\0'."
		);
	}

	generate_encode_from_bytes(_part_count);
	generate_strings();
}

u8char::u8char(const std::vector<char8_t>& utf8_char) {
	int s = utf8_char.size();
	if (s > 4) {
		throw u8exception(
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

u8char::u8char(const char* const utf8_char)
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
		throw u8exception(
			U8_EXCEPTION_TYPE::INVAILD_UTF8_ENCODE,
			"The char array passed as a utf8 char needs to be within 4 bytes and end with '\\0'."
		);
	}

	generate_encode_from_bytes(_part_count);
	generate_strings();
}

u8char::u8char(const std::vector<char>& utf8_char) {
	int s = utf8_char.size();
	if (s > 4) {
		throw u8exception(
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

unsigned short u8char::generate_bytes_from_encode() {
	if (!is_vaild_encode()) {
		clear();
		throw u8exception(
			U8_EXCEPTION_TYPE::INVAILD_UTF8_ENCODE,
			"The character range of utf-8 needs to be between 0 and 0xEFBFBFBF."
		);
	}

	unsigned short byte_count = 1;
	for (int i = 3; i >= 0; --i)
	{
		_parts[i] = (char8_t)(_encode >> (8 * i)) & (char8_t)0xFF;
		if (_parts[i] != 0)
			byte_count = i+1;
	}
	return byte_count;
}

void u8char::generate_encode_from_bytes(unsigned short byte_count) {
	_encode = 0;
	for (int i = 0; i < byte_count; ++i)
	{
		_encode |= _parts[i] << (8 * (4-1-i));
	}

	if (!is_vaild_encode()) {
		clear();
		throw u8exception(
			U8_EXCEPTION_TYPE::INVAILD_UTF8_ENCODE,
			"The character range of utf-8 needs to be between 0 and 0xEFBFBFBF."
		);
	}
}

void u8char::clear()
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