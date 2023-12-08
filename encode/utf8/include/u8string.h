#ifndef __U8STRING_H__
#define __U8STRING_H__

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

			size_t size() {
				return _string.size();
			}
		private:
			std::vector<u8char> _string;
		};
	}
}

#endif