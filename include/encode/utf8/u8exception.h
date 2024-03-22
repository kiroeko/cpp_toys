#ifndef __U8EXCEPTION_H__
#define __U8EXCEPTION_H__

#include <string>
#include <stdexcept>

namespace liquid
{
	namespace encode
	{
		enum U8_EXCEPTION_TYPE
		{
			INVAILD_UTF8_ENCODE
		};

		class u8exception : public std::exception
		{
		public:
			u8exception(U8_EXCEPTION_TYPE reason, const std::string message) :
				_reason(reason), _message(message) {}

			U8_EXCEPTION_TYPE reason()
			{
				return _reason;
			}

			const char* what() const noexcept override {
				return _message.c_str();
			}
		private:
			U8_EXCEPTION_TYPE _reason;
			std::string _message;
		};
	}
}

#endif