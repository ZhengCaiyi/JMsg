#ifndef JMSG_UTIL_H
#define JMSG_UTIL_H
#include <string>
#include "jmsg_encodeable.h"
bool JMsgIsDigit(char c);
bool JMsgIsAlpha(char c);
bool JMsgIsUnderLine(char c);
bool JMsgIsEmptyChar(char c);
bool JMsgIsChangeLine(char c);
bool JMsgGetFileString(const std::string& path, std::string& data);
int JMsgEcodeSize(int sizeInput, unsigned char* buf);
int JMsgDecodeSize(unsigned char* buf, int* sizeLen, unsigned char* endPtr);
std::string JMsgGetFileNameWithoutExt(const std::string& fileName);
std::string JmsgGetFilePath(const std::string& fileName);
std::string JmsgGetFileName(const std::string& fileName);
std::string JMsgGetFileExt(const std::string& fileName);
std::string JMsgGetFirstCharBigerCase(const std::string& data);
std::string JMsgGetFormatString(const char* format, ...);
void JMsgAppendFormatString(std::string&, const char* format, ...);
std::string JMsgToJson(IJMsgEncodeable& msg);
bool JMsgFromJson(const std::string& json, IJMsgEncodeable & msg);
#endif
