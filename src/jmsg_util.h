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
#endif