#ifndef JMSG_UTIL_H
#define JMSG_UTIL_H
#include <string>
bool jMsgIsDigit(char c);
bool jMsgIsAlpha(char c);
bool jMsgIsUnderLine(char c);
bool jMsgIsEmptyChar(char c);
bool jMsgIsChangeLine(char c);
std::string* jMsgGetFileString(const std::string& path);
int jMsgEcodeSize(int sizeInput, unsigned char* buf);
int jMsgDecodeSize(unsigned char* buf, int* sizeLen, unsigned char* endPtr);
std::string JMsgGetFileNameWithoutExt(const std::string& fileName);
std::string JmsgGetFilePath(const std::string& fileName);
std::string JmsgGetFileName(const std::string& fileName);
std::string JMsgGetFileExt(const std::string& fileName);
const std::string& JMsgBase64Encde( const char* pData,int nDataLen, std::string& strValue);
int JMsgBase64Decode(const char* pData,size_t nDataLen,void* pDecodeData);
std::string JMsgGetFirstCharBiggerCase(const std::string& data);
#if defined(__GNUC__)

#define PRINTF_FORMAT(format_param, dots_param) \

__attribute__((format(printf, format_param, dots_param)))
#else

#define PRINTF_FORMAT(format_param, dots_param)

#endif


std::string jMsgGetFormatString(const char* format, ...) PRINTF_FORMAT(1, 2);

void jMsgAppendFormatString(std::string&, const char* format, ...) PRINTF_FORMAT(2, 3);

#endif
