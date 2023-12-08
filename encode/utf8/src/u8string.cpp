#include <ranges>
#include "u8string.h"
#include "u8exception.h"

using namespace liquid::encode;

u8string::u8string(const std::string string)
{
    auto u8slice = [](auto c1, auto c2) { return (0b11000000 & char(c2)) == 0b10000000; };
    auto u8charlist = string | std::ranges::views::chunk_by(u8slice);

    for (const auto& u8ch : u8charlist)
    {
        std::vector<char8_t> u8charparts;
        for (const char8_t& u8charpart : u8ch)
        {
            u8charparts.push_back(u8charpart);
        }

        u8char c(u8charparts);
        _string.push_back(c);
    }
}

u8string::u8string(const std::wstring wide_string)
{
    auto u8slice = [](auto c1, auto c2) { return (0b11000000 & char(c2)) == 0b10000000; };
    auto u8charlist = wide_string | std::ranges::views::chunk_by(u8slice);

    for (const auto& u8ch : u8charlist)
    {
        std::vector<char8_t> u8charparts;
        for (const char8_t& u8charpart : u8ch)
        {
            u8charparts.push_back(u8charpart);
        }

        u8char c(u8charparts);
        _string.push_back(c);
    }
}

u8string::u8string(const std::u8string utf8_string)
{
    auto u8slice = [](auto c1, auto c2) { return (0b11000000 & char(c2)) == 0b10000000; };
    auto u8charlist = utf8_string | std::ranges::views::chunk_by(u8slice);

    for (const auto& u8ch : u8charlist)
    {
        std::vector<char8_t> u8charparts;
        for (const char8_t& u8charpart : u8ch)
        {
            u8charparts.push_back(u8charpart);
        }

        u8char c(u8charparts);
        _string.push_back(c);
    }
}