#include <ranges>
#include "u8string.h"
#include "u8exception.h"

using namespace liquid::encode;

u8string::u8string(const std::u8string& string)
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

u8string::u8string(const std::vector<u8char>& string)
{
    _string = string;
}

std::vector<u8string> u8string::split(const u8char& delimiter)
{
    std::vector<u8string> result;
    if (_string.empty())
        return result;

    auto iter = _string.begin();
    while (true)
    {
        std::vector<u8char> part;
        auto part_end = std::find(iter, _string.end(), delimiter);
        for (auto i = iter; i < part_end; ++i)
        {
            part.push_back(*i);
        }
        u8string part_str(part);
        result.push_back(part_str);

        if (part_end >= _string.end() - 1)
            break;
        iter = part_end + 1;
    }

    return result;
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