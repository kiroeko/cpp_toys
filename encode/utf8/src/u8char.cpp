#include "u8char.h"
#include "u8exception.h"

using namespace liquid::encode;

u8char::u8char(const char& c) {
	encode = c;
	init_from_encode();
}

u8char::u8char(const wchar_t& c) {
	encode = c;
	init_from_encode();
}

u8char::u8char(const char8_t& c) {
	encode = c;
	init_from_encode();
}

u8char::u8char(const char8_t& c0, const char8_t& c1)
{
	byte1 = c1;
	byte0 = c0;
	init_from_bytes();
}

u8char::u8char(const char8_t& c0, const char8_t& c1, const char8_t& c2)
{
	byte2 = c2;
	byte1 = c1;
	byte0 = c0;
	init_from_bytes();
}

u8char::u8char(const char8_t& c0, const char8_t& c1, const char8_t& c2, const char8_t& c3)
{
	byte3 = c3;
	byte2 = c2;
	byte1 = c1;
	byte0 = c0;
	init_from_bytes();
}

u8char::u8char(const unsigned long& c) {
	encode = c;
	init_from_encode();
}

u8char::u8char(const std::vector<char8_t>& c) {
	if (c.size() > 4) {
		throw u8exception(
			U8_EXCEPTION_TYPE::INVAILD_UTF8_ENCODE,
			"A utf-8 character needs to be within 4 bytes."
		);
	}

	int s = c.size();
	switch (s)
	{
	case 4:
		byte3 = c[3];
		[[fallthrough]];
	case 3:
		byte2 = c[2];
		[[fallthrough]];
	case 2:
		byte1 = c[1];
		[[fallthrough]];
	case 1:
		byte0 = c[0];
	}

	init_from_bytes();
}

bool u8char::is_vaild_encode(unsigned long encode) {
	if (0 <= encode && encode <= 0x10FFFF)
		return true;
	return false;
}

void u8char::init_from_encode() {
	if (is_vaild_encode(encode)) {
		clear();
		throw u8exception(
			U8_EXCEPTION_TYPE::INVAILD_UTF8_ENCODE,
			"The character range of utf-8 needs to be between 0 and 0x10FFFF."
		);
	}

	byte3 = (unsigned char)(encode >> 24) & (unsigned char)0xFF;
	byte2 = (unsigned char)(encode >> 16) & (unsigned char)0xFF;
	byte1 = (unsigned char)(encode >> 8) & (unsigned char)0xFF;
	byte0 = (unsigned char)encode & (unsigned char)0xFF;
}

void u8char::init_from_bytes() {
	encode = 0;
	encode |= (byte3 << 24);
	encode |= (byte2 << 16);
	encode |= (byte1 << 8);
	encode |= byte0;

	if (is_vaild_encode(encode)) {
		clear();
		throw u8exception(
			U8_EXCEPTION_TYPE::INVAILD_UTF8_ENCODE,
			"The character range of utf-8 needs to be between 0 and 0x10FFFF."
		);
	}
}

std::string u8char::convert_to_str()
{
	std::string str;

	for (const auto& c: ) {
        s.push_back(cc);
    }
	str
}

std::wstring u8char::convert_to_wstr()
{

}

std::u8string u8char::convert_to_u8str()
{

}

void u8char::clear()
{
	byte3 = 0;
	byte2 = 0;
	byte1 = 0;
	byte0 = 0;
	encode = 0;
	str.clear();
	wstr.clear();
	u8str.clear();
}