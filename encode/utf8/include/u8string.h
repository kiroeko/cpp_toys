#ifndef __U8STRING_H__
#define __U8STRING_H__

#include <iostream>
#include <vector>
#include <string>
#include "u8char.h"

namespace liquid {
	namespace encode {
		class u8string
		{
		public:
			u8string(const std::string string);
			u8string(const std::wstring wide_string);
			u8string(const std::u8string utf8_string);

			bool empty()
			{
				return _string.size() == 0;
			}

			size_t size() {
				return _string.size();
			}

			u8char operator[](size_t index) {
				return _string[index];
			}


			std::ostream& operator<<(std::ostream& os) const {
				for (const auto& c : _string)
				{
				}
				return os;
			}

			std::istream& operator>>(std::istream& is);
		private:
			inline void clear()
			{
				_string.clear();
			}

			std::vector<u8char> _string;
		};
	}
}

#endif