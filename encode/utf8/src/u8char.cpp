#include "u8char.h"
#include "u8exception.h"
#include "u8string.h"

using namespace liquid::encode;

u8char::u8char(const char8_t& utf8_char) {
	_encode = utf8_char;
	generate_bytes_from_encode();
	_byte_count = 1;
	generate_strings();
}

u8char::u8char(const unsigned long& utf8_encode) {
	_encode = utf8_encode;
	_byte_count = generate_bytes_from_encode();
	generate_strings();
}

u8char::u8char(const unsigned char& utf8_char) {
	_encode = utf8_char;
	generate_bytes_from_encode();
	_byte_count = 1;
	generate_strings();
}

u8char::u8char(const wchar_t& utf8_char) {
	_encode = utf8_char;
	_byte_count = generate_bytes_from_encode();
	generate_strings();
}

u8char::u8char(const char8_t& byte0, const char8_t& byte1)
{
	_bytes[0] = byte0;
	_bytes[1] = byte1;
	_byte_count = 2;
	generate_encode_from_bytes(_byte_count);
	generate_strings();
}

u8char::u8char(const char8_t& byte0, const char8_t& byte1, const char8_t& byte2)
{
	_bytes[0] = byte0;
	_bytes[1] = byte1;
	_bytes[2] = byte2;
	_byte_count = 3;
	generate_encode_from_bytes(_byte_count);
	generate_strings();
}

u8char::u8char(const char8_t& byte0, const char8_t& byte1, const char8_t& byte2, const char8_t& byte3)
{
	_bytes[0] = byte0;
	_bytes[1] = byte1;
	_bytes[2] = byte2;
	_bytes[3] = byte3;
	_byte_count = 4;
	generate_encode_from_bytes(_byte_count);
	generate_strings();
}

u8char::u8char(const char8_t* const utf8_char, unsigned short byte_count)
{
	if (byte_count > 4) {
		throw u8exception(
			U8_EXCEPTION_TYPE::INVAILD_UTF8_ENCODE,
			"A utf-8 character needs to be within 4 bytes."
		);
	}

	for (int i = 0; i < byte_count; ++i)
	{
		_bytes[i] = utf8_char[i];
	}
	_byte_count = byte_count;
	generate_encode_from_bytes(_byte_count);
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
		_bytes[i] = utf8_char[i];
	}
	_byte_count = s;
	generate_encode_from_bytes(_byte_count);
	generate_strings();
}

std::istream& u8char::operator>>(std::istream& is)
{
	std::string tmp;
	is >> tmp;
	u8string s(tmp);

	return is;
}

unsigned short u8char::generate_bytes_from_encode() {
	if (is_vaild_encode()) {
		clear();
		throw u8exception(
			U8_EXCEPTION_TYPE::INVAILD_UTF8_ENCODE,
			"The character range of utf-8 needs to be between 0 and 0x10FFFF."
		);
	}

	unsigned short byte_count = 1;
	for (int i = 3; i >= 0; --i)
	{
		_bytes[i] = (char8_t)(_encode >> (8 * i)) & (char8_t)0xFF;
		if (_bytes[i] != 0)
			byte_count = i+1;
	}
	return byte_count;
}

void u8char::generate_encode_from_bytes(unsigned short byte_count) {
	_encode = 0;
	for (int i = 0; i < byte_count; ++i)
	{
		_encode |= _bytes[i] << (8 * i);
	}

	if (is_vaild_encode()) {
		clear();
		throw u8exception(
			U8_EXCEPTION_TYPE::INVAILD_UTF8_ENCODE,
			"The character range of utf-8 needs to be between 0 and 0x10FFFF."
		);
	}
}

void u8char::clear()
{
	for (char8_t& b : _bytes)
	{
		b = 0;
	}
	_byte_count = 0;
	_encode = 0;

	_string_representation.clear();
	_wstring_representation.clear();
	_u8string_representation.clear();
}