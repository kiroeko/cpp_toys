#ifndef __U8STRING_H__
#define __U8STRING_H__

#if __cplusplus >= 201700L && __cplusplus < 202000L
// Implement for the version C++17.

#include <iostream>
#include <functional>
#include <ranges>
#include <string>
#include <vector>
#include "u8char.h"

namespace liquid::encode
{
	class u8string
	{
	public:
		u8string() : u8string("") {}

		u8string(const std::string& string)
		{
			std::vector<std::vector<char>> u8char_list;

			auto lastIter = string.begin();
			auto iter = string.begin();
			while (true)
			{
				bool needBreak = false;
				bool needCollect = false;
				if (iter == string.end())
				{
					needBreak = true;
					needCollect = true;
				}
				else
				{
					const char& c = *iter;
					if (!((0b11000000 & c) == 0b10000000))
					{
						needCollect = true;
					}
				}

				if (needCollect)
				{
					std::vector<char> utf8char(lastIter, iter);
					if (!utf8char.empty())
					{
						u8char_list.push_back(utf8char);
					}
					lastIter = iter;
				}

				if (needBreak)
					break;
				++iter;
			}

			for (const auto& temp_u8char : u8char_list)
			{
				size_t count = temp_u8char.size();
				if (count > 4)
				{
					clear();
					throw u8exception(
						U8_EXCEPTION_TYPE::INVAILD_UTF8_ENCODE,
						"A utf-8 character needs to be within 4 bytes."
					);
				}

				std::vector<char> u8char_parts;
				for (const char& u8char_part : temp_u8char)
				{
					u8char_parts.push_back(u8char_part);
				}

				u8char c(u8char_parts);
				_string.push_back(c);
			}
		}

		u8string(const char* const string) : u8string((std::string)string) {}

		u8string(const std::vector<char>& string) : u8string(string.data()) {}

		u8string(const std::vector<u8char>& string)
		{
			_string = string;
		}

		std::vector<u8string> split(const u8char& delimiter)
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

		u8string sub_string(size_t len) const
		{
			return sub_string(0, len);
		}

		u8string sub_string(size_t start_index, size_t len) const
		{
			size_t end_index = start_index + len;
			size_t max = _string.size();
			if (end_index > max)
				end_index = max;

			std::vector<u8char> substr;
			for (size_t i = start_index; i < end_index; ++i)
			{
				substr.push_back(_string[i]);
			}
			return u8string(substr);
		}

		u8string replace(u8char old_char, u8char new_char, bool replace_all = true) const
		{
			return replace(new_char, [&old_char](const u8char& c) { return c == old_char; }, replace_all);
		}

		u8string replace(u8char new_char, std::function<bool(const u8char&)> func, bool replace_all = true) const
		{
			u8string copy_str(*this);
			if (func == nullptr)
				return copy_str;

			size_t max = copy_str.size();
			for (size_t i = 0; i < max; ++i)
			{
				if (func(copy_str[i]))
				{
					copy_str[i] = new_char;
					if (!replace_all)
						break;
				}
			}
			return copy_str;
		}

		u8string replace(u8string old_string, u8string new_string, bool replace_all = true) const
		{
			if (old_string.empty())
				return *this;

			u8string copy_str(*this);
			if (copy_str.empty())
				return copy_str;
			size_t copy_str_size = copy_str.size();
			size_t old_str_size = old_string.size();
			if (copy_str_size < old_str_size)
				return copy_str;

			for (size_t i = 0; i <= (copy_str_size - old_str_size); ++i)
			{
				if (std::equal(copy_str._string.begin()+i, copy_str._string.end()+i+old_str_size, old_string._string.begin()))
				{
					std::copy(old_string._string.begin(), old_string._string.end(), copy_str._string.begin()+i);
					if (!replace_all)
						break;
				}
			}
		}

		u8char* find(std::function<bool(const u8char&)> func)
		{
			if (func == nullptr)
				return nullptr;

			for (auto& c : _string)
			{
				if (func(c))
				{
					return &c;
				}
			}
			return nullptr;
		}

		std::vector<u8char*> find_all(std::function<bool(const u8char&)> func)
		{
			if (func == nullptr)
				return std::vector<u8char*>();

			std::vector<u8char*> find_results;
			for (auto& c : _string)
			{
				if (func(c))
				{
					find_results.push_back(&c);
				}
			}
			return find_results;
		}

		u8string append(const u8string& u8str2)
		{
			return *this += u8str2;
		}

		bool empty() const
		{
			return _string.size() == 0;
		}

		void clear()
		{
			_string.clear();
		}

		size_t size() const
		{
			return _string.size();
		}

		std::string to_string() const
		{
			std::string tmp;
			for (const auto& c : _string)
			{
				tmp += c.to_string();
			}
			return tmp;
		}

		u8char& operator[](size_t index)
		{
			return _string[index];
		}

		bool operator==(const u8char& other) const
		{
			return to_string() == other.to_string();
		}

		bool operator!=(const u8char& other) const
		{
			return to_string() != other.to_string();
		}

		friend u8string operator+(const u8string& u8str1, const u8string& u8str2)
		{
			std::vector<u8char> tmp = u8str1._string;
			for (const auto& c : u8str2._string)
			{
				tmp.push_back(c);
			}
			u8string tmp_str(tmp);
			return tmp;
		}

		friend u8string operator+=(u8string& u8str, const u8char& utf8_char)
		{
			u8str._string.push_back(utf8_char);
			return u8str;
		}

		friend u8string operator+=(u8string& u8str, const u8string& u8str2)
		{
			u8str._string.insert(u8str._string.end(), u8str2._string.begin(), u8str2._string.end());
			return u8str;
		}

		friend std::ostream& operator<<(std::ostream& os, const u8string& utf8_string)
		{
			for (const auto& c : utf8_string._string)
			{
				os << c;
			}
			return os;
		}

		friend std::istream& operator>>(std::istream& is, u8string& utf8_string)
		{
			std::string tmp;
			is >> tmp;

			u8string s(tmp);
			utf8_string = s;

			return is;
		}

		auto begin()
		{
			return _string.begin();
		}

		auto end()
		{
			return _string.end();
		}

		auto rbegin()
		{
			return _string.rbegin();
		}

		auto rend()
		{
			return _string.rend();
		}

		auto cbegin() const
		{
			return _string.cbegin();
		}

		auto cend() const
		{
			return _string.cend();
		}

		auto crbegin() const
		{
			return _string.crbegin();
		}

		auto crend() const
		{
			return _string.crend();
		}

		operator std::string() const
		{
			return to_string();
		}
	private:
		std::vector<u8char> _string;
	};
}
#elif __cplusplus >= 202000L && __cplusplus < 202300L
// Implement for the version C++20.

#include <iostream>
#include <functional>
#include <ranges>
#include <string>
#include <vector>
#include "u8char.h"

namespace liquid::encode
{
	class u8string
	{
	public:
		u8string() : u8string("") {}

		u8string(const std::u8string& string)
		{
			std::vector<std::vector<char8_t>> u8char_list;

			auto lastIter = string.begin();
			auto iter = string.begin();
			while (true)
			{
				bool needBreak = false;
				bool needCollect = false;
				if (iter == string.end())
				{
					needBreak = true;
					needCollect = true;
				}
				else
				{
					const char8_t& c = *iter;
					if (!((0b11000000 & c) == 0b10000000))
					{
						needCollect = true;
					}
				}

				if (needCollect)
				{
					std::vector<char8_t> utf8char(lastIter, iter);
					if (!utf8char.empty())
					{
						u8char_list.push_back(utf8char);
					}
					lastIter = iter;
				}

				if (needBreak)
					break;
				++iter;
			}

			for (const auto& temp_u8char : u8char_list)
			{
				size_t count = temp_u8char.size();
				if (count > 4)
				{
					clear();
					throw u8exception (
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

		u8string(const char8_t* const string) : u8string((std::u8string)string) {}

		u8string(const std::vector<char8_t>& string) : u8string(string.data()) {}

		u8string(const std::string& string)
		{
			std::vector<std::vector<char>> u8char_list;

			auto lastIter = string.begin();
			auto iter = string.begin();
			while (true)
			{
				bool needBreak = false;
				bool needCollect = false;
				if (iter == string.end())
				{
					needBreak = true;
					needCollect = true;
				}
				else
				{
					const char& c = *iter;
					if (!((0b11000000 & c) == 0b10000000))
					{
						needCollect = true;
					}
				}

				if (needCollect)
				{
					std::vector<char> utf8char(lastIter, iter);
					if (!utf8char.empty())
					{
						u8char_list.push_back(utf8char);
					}
					lastIter = iter;
				}

				if (needBreak)
					break;
				++iter;
			}

			for (const auto& temp_u8char : u8char_list)
			{
				unsigned int count = std::ranges::distance(temp_u8char);
				if (count > 4)
				{
					clear();
					throw u8exception (
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

		u8string(const char* const string) : u8string((std::string)string) {}

		u8string(const std::vector<char>& string) : u8string(string.data()) {}

		u8string(const std::vector<u8char>& string)
		{
			_string = string;
		}

		std::vector<u8string> split(const u8char& delimiter)
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

		u8string sub_string(size_t len) const
		{
			return sub_string(0, len);
		}

		u8string sub_string(size_t start_index, size_t len) const
		{
			size_t end_index = start_index + len;
			size_t max = _string.size();
			if (end_index > max)
				end_index = max;

			std::vector<u8char> substr;
			for (size_t i = start_index; i < end_index; ++i)
			{
				substr.push_back(_string[i]);
			}
			return u8string(substr);
		}

		u8string replace(u8char old_char, u8char new_char, bool replace_all = true) const
		{
			return replace(new_char, [&old_char](const u8char& c) { return c == old_char; }, replace_all);
		}

		u8string replace(u8char new_char, std::function<bool(const u8char&)> func, bool replace_all = true) const
		{
			u8string copy_str(*this);
			if (func == nullptr)
				return copy_str;

			size_t max = copy_str.size();
			for (size_t i = 0; i < max; ++i)
			{
				if (func(copy_str[i]))
				{
					copy_str[i] = new_char;
					if (!replace_all)
						break;
				}
			}
			return copy_str;
		}

		u8string replace(u8string old_string, u8string new_string, bool replace_all = true) const
		{
			if (old_string.empty())
				return *this;

			u8string copy_str(*this);
			if (copy_str.empty())
				return copy_str;
			size_t copy_str_size = copy_str.size();
			size_t old_str_size = old_string.size();
			if (copy_str_size < old_str_size)
				return copy_str;

			for (size_t i = 0; i <= (copy_str_size - old_str_size); ++i)
			{
				if (std::equal(copy_str._string.begin() + i, copy_str._string.end() + i + old_str_size, old_string._string.begin()))
				{
					std::copy(old_string._string.begin(), old_string._string.end(), copy_str._string.begin() + i);
					if (!replace_all)
						break;
				}
			}
		}

		u8char* find(std::function<bool(const u8char&)> func)
		{
			if (func == nullptr)
				return nullptr;

			for (auto& c : _string)
			{
				if (func(c))
				{
					return &c;
				}
			}
			return nullptr;
		}

		std::vector<u8char*> find_all(std::function<bool(const u8char&)> func)
		{
			if (func == nullptr)
				return std::vector<u8char*>();

			std::vector<u8char*> find_results;
			for (auto& c : _string)
			{
				if (func(c))
				{
					find_results.push_back(&c);
				}
			}
			return find_results;
		}

		u8string append(const u8string& u8str2)
		{
			return *this += u8str2;
		}

		bool empty() const
		{
			return _string.size() == 0;
		}

		void clear()
		{
			_string.clear();
		}

		size_t size() const
		{
			return _string.size();
		}

		std::string to_string() const
		{
			std::string tmp;
			for (const auto& c : _string)
			{
				tmp += c.to_string();
			}
			return tmp;
		}

		std::u8string to_u8string() const
		{
			std::u8string tmp;
			for (const auto& c : _string)
			{
				tmp += c.to_u8string();
			}
			return tmp;
		}

		u8char& operator[](size_t index)
		{
			return _string[index];
		}

		bool operator==(const u8char& other) const
		{
			return to_string() == other.to_string();
		}

		bool operator!=(const u8char& other) const
		{
			return to_string() != other.to_string();
		}

		friend u8string operator+(const u8string& u8str1, const u8string& u8str2)
		{
			std::vector<u8char> tmp = u8str1._string;
			for (const auto& c : u8str2._string)
			{
				tmp.push_back(c);
			}
			u8string tmp_str(tmp);
			return tmp;
		}

		friend u8string operator+=(u8string& u8str, const u8char& utf8_char)
		{
			u8str._string.push_back(utf8_char);
			return u8str;
		}

		friend u8string operator+=(u8string& u8str, const u8string& u8str2)
		{
			u8str._string.insert(u8str._string.end(), u8str2._string.begin(), u8str2._string.end());
			return u8str;
		}

		friend std::ostream& operator<<(std::ostream& os, const u8string& utf8_string)
		{
			for (const auto& c : utf8_string._string)
			{
				os << c;
			}
			return os;
		}

		friend std::istream& operator>>(std::istream& is, u8string& utf8_string)
		{
			std::string tmp;
			is >> tmp;

			u8string s(tmp);
			utf8_string = s;

			return is;
		}

		auto begin()
		{
			return _string.begin();
		}

		auto end()
		{
			return _string.end();
		}

		auto rbegin()
		{
			return _string.rbegin();
		}

		auto rend()
		{
			return _string.rend();
		}

		auto cbegin() const
		{
			return _string.cbegin();
		}

		auto cend() const
		{
			return _string.cend();
		}

		auto crbegin() const
		{
			return _string.crbegin();
		}

		auto crend() const
		{
			return _string.crend();
		}

		operator std::string() const
		{
			return to_string();
		}

		operator std::u8string() const
		{
			return to_u8string();
		}
	private:
		std::vector<u8char> _string;
	};
}
#else
// Implement for versions C++23 and later.

#include <iostream>
#include <functional>
#include <ranges>
#include <string>
#include <vector>
#include "u8char.h"

namespace liquid::encode
{
	class u8string
	{
	public:
		u8string() : u8string("") {}

		u8string(const std::u8string& string)
		{
			auto u8slice = [](const auto& c1, const auto& c2) { return (0b11000000 & c2) == 0b10000000; };
			auto u8char_list = string | std::ranges::views::chunk_by(u8slice);

			for (const auto& temp_u8char : u8char_list)
			{
				unsigned int count = std::ranges::distance(temp_u8char);
				if (count > 4)
				{
					clear();
					throw u8exception (
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

		u8string(const char8_t* const string) : u8string((std::u8string)string) {}

		u8string(const std::vector<char8_t>& string) : u8string(string.data()) {}

		u8string(const std::string& string)
		{
			auto u8slice = [](const auto& c1, const auto& c2) { return (0b11000000 & c2) == 0b10000000; };
			auto u8char_list = string | std::ranges::views::chunk_by(u8slice);

			for (const auto& temp_u8char : u8char_list)
			{
				unsigned int count = std::ranges::distance(temp_u8char);
				if (count > 4)
				{
					clear();
					throw u8exception (
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

		u8string(const char* const string) : u8string((std::string)string) {}

		u8string(const std::vector<char>& string) : u8string(string.data()) {}

		u8string(const std::vector<u8char>& string)
		{
			_string = string;
		}

		std::vector<u8string> split(const u8char& delimiter)
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

		u8string sub_string(size_t len) const
		{
			return sub_string(0, len);
		}

		u8string sub_string(size_t start_index, size_t len) const
		{
			size_t end_index = start_index + len;
			size_t max = _string.size();
			if (end_index > max)
				end_index = max;

			std::vector<u8char> substr;
			for (size_t i = start_index; i < end_index; ++i)
			{
				substr.push_back(_string[i]);
			}
			return u8string(substr);
		}

		u8string replace(u8char old_char, u8char new_char, bool replace_all = true) const
		{
			return replace(new_char, [&old_char](const u8char& c) { return c == old_char; }, replace_all);
		}

		u8string replace(u8char new_char, std::function<bool(const u8char&)> func, bool replace_all = true) const
		{
			u8string copy_str(*this);
			if (func == nullptr)
				return copy_str;

			size_t max = copy_str.size();
			for (size_t i = 0; i < max; ++i)
			{
				if (func(copy_str[i]))
				{
					copy_str[i] = new_char;
					if (!replace_all)
						break;
				}
			}
			return copy_str;
		}

		u8string replace(u8string old_string, u8string new_string, bool replace_all = true) const
		{
			if (old_string.empty())
				return *this;

			u8string copy_str(*this);
			if (copy_str.empty())
				return copy_str;
			size_t copy_str_size = copy_str.size();
			size_t old_str_size = old_string.size();
			if (copy_str_size < old_str_size)
				return copy_str;

			for (size_t i = 0; i <= (copy_str_size - old_str_size); ++i)
			{
				if (std::equal(copy_str._string.begin() + i, copy_str._string.end() + i + old_str_size, old_string._string.begin()))
				{
					std::copy(old_string._string.begin(), old_string._string.end(), copy_str._string.begin() + i);
					if (!replace_all)
						break;
				}
			}
		}

		u8char* find(std::function<bool(const u8char&)> func)
		{
			if (func == nullptr)
				return nullptr;

			for (auto& c : _string)
			{
				if (func(c))
				{
					return &c;
				}
			}
			return nullptr;
		}

		std::vector<u8char*> find_all(std::function<bool(const u8char&)> func)
		{
			if (func == nullptr)
				return std::vector<u8char*>();

			std::vector<u8char*> find_results;
			for (auto& c : _string)
			{
				if (func(c))
				{
					find_results.push_back(&c);
				}
			}
			return find_results;
		}

		u8string append(const u8string& u8str2)
		{
			return *this += u8str2;
		}

		bool empty() const
		{
			return _string.size() == 0;
		}

		void clear()
		{
			_string.clear();
		}

		size_t size() const
		{
			return _string.size();
		}

		std::string to_string() const
		{
			std::string tmp;
			for (const auto& c : _string)
			{
				tmp += c.to_string();
			}
			return tmp;
		}

		std::u8string to_u8string() const
		{
			std::u8string tmp;
			for (const auto& c : _string)
			{
				tmp += c.to_u8string();
			}
			return tmp;
		}

		u8char& operator[](size_t index)
		{
			return _string[index];
		}

		bool operator==(const u8char& other) const
		{
			return to_string() == other.to_string();
		}

		bool operator!=(const u8char& other) const
		{
			return to_string() != other.to_string();
		}

		friend u8string operator+(const u8string& u8str1, const u8string& u8str2)
		{
			std::vector<u8char> tmp = u8str1._string;
			for (const auto& c : u8str2._string)
			{
				tmp.push_back(c);
			}
			u8string tmp_str(tmp);
			return tmp;
		}

		friend u8string operator+=(u8string& u8str, const u8char& utf8_char)
		{
			u8str._string.push_back(utf8_char);
			return u8str;
		}

		friend u8string operator+=(u8string& u8str, const u8string& u8str2)
		{
			u8str._string.append_range(u8str2._string);
			return u8str;
		}

		friend std::ostream& operator<<(std::ostream& os, const u8string& utf8_string)
		{
			for (const auto& c : utf8_string._string)
			{
				os << c;
			}
			return os;
		}

		friend std::istream& operator>>(std::istream& is, u8string& utf8_string)
		{
			std::string tmp;
			is >> tmp;

			u8string s(tmp);
			utf8_string = s;

			return is;
		}

		auto begin()
		{
			return _string.begin();
		}

		auto end()
		{
			return _string.end();
		}

		auto rbegin()
		{
			return _string.rbegin();
		}

		auto rend()
		{
			return _string.rend();
		}

		auto cbegin() const
		{
			return _string.cbegin();
		}

		auto cend() const
		{
			return _string.cend();
		}

		auto crbegin() const
		{
			return _string.crbegin();
		}

		auto crend() const
		{
			return _string.crend();
		}

		operator std::string() const
		{
			return to_string();
		}

		operator std::u8string() const
		{
			return to_u8string();
		}
	private:
		std::vector<u8char> _string;
	};
}
#endif

namespace liquid::encode
{
	u8string operator""_utf8str(const char* str, std::size_t size)
	{
		return u8string(std::string(str, size));
	}
}

#endif