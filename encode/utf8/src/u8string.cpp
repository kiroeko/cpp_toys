#include <ranges>
#include "u8string.h"
#include "u8exception.h"

using namespace liquid::encode;

u8string::u8string(const std::string& string)
{
    auto u8slice = [](auto c1, auto c2) { return (0b11000000 & char(c2)) == 0b10000000; };
    auto u8char_list = string | std::ranges::views::chunk_by(u8slice);

    for (const auto& temp_u8char : u8char_list)
    {
        unsigned long count = std::ranges::distance(temp_u8char);
        if (count > 4)
        {
            clear();
            throw u8exception(
                U8_EXCEPTION_TYPE::INVAILD_UTF8_ENCODE,
                "A utf-8 character needs to be within 4 bytes."
            );
        }

        std::vector<char8_t> u8char_parts;
        for (const char8_t& u8char_part : temp_u8char)
        {
            u8char_parts.push_back(u8char_part);
        }

        u8char c(u8char_parts);
        _string.push_back(c);
    }
}

u8string::u8string(const std::u8string& utf8_string)
{
    auto u8slice = [](auto c1, auto c2) { return (0b11000000 & char(c2)) == 0b10000000; };
    auto u8char_list = utf8_string | std::ranges::views::chunk_by(u8slice);

    for (const auto& temp_u8char : u8char_list)
    {
        unsigned long count = std::ranges::distance(temp_u8char);
        if (count > 4)
        {
            clear();
            throw u8exception(
                U8_EXCEPTION_TYPE::INVAILD_UTF8_ENCODE,
                "A utf-8 character needs to be within 4 bytes."
            );
        }

        std::vector<char8_t> u8char_parts;
        for (const char8_t& u8char_part : temp_u8char)
        {
            u8char_parts.push_back(u8char_part);
        }

        u8char c(u8char_parts);
        _string.push_back(c);
    }
}

std::string u8string::to_string() const
{
    std::string tmp;
    for (const auto& c : _string) {
        tmp += c.to_string();
    }
    return tmp;
}

std::u8string u8string::to_u8string() const
{
    std::u8string tmp;
    for (const auto& c : _string) {
        tmp += c.to_u8string();
    }
    return tmp;
}