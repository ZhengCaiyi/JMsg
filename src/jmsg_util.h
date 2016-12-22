#ifndef JMSG_UTIL_H
#define JMSG_UTIL_H
#include <string>
bool jMsgIsDigit(char c);
bool jMsgIsAlpha(char c);
bool jMsgIsUnderLine(char c);
bool jMsgIsEmptyChar(char c);
std::string* jMsgGetFileString(const std::string& path);
int jMsgEcodeSize(int sizeInput, unsigned char* buf);
int jMsgDecodeSize(unsigned char* buf, int* sizeLen);



#if defined(__GNUC__)

#define PRINTF_FORMAT(format_param, dots_param) \

__attribute__((format(printf, format_param, dots_param)))
#else

#define PRINTF_FORMAT(format_param, dots_param)

#endif


std::string jMsgGetFormatString(const char* format, ...) PRINTF_FORMAT(1, 2);

void jMsgAppendFormatString(std::string&, const char* format, ...) PRINTF_FORMAT(2, 3);

#endif
