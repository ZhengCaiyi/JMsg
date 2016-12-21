#include "stdio.h"
#include "jmsg_util.h"
using namespace std;
bool jMsgIsDigit(char c) {
	return c >= '0' && c <= '9';
}

bool jMsgIsAlpha(char c) {
	return c >= 'A' && c <= 'z';
}

bool jMsgIsUnderLine(char c) {
	return c == '_';
}

bool jMsgIsEmptyChar(char c) {
	return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

std::string* jMsgGetFileString(const std::string& path) {
	string* data = new string;
	FILE* file;
	file = fopen(path.c_str(), "rb");

	if (!file) {
		return data;
	}

	char readBuffer[4096] = {};

	while (!feof(file)) {
		size_t len = fread(readBuffer, 1, sizeof(readBuffer), file);
		data->append(readBuffer, len);
	}
	fclose(file);
	return data;
}

int jMsgEcodeSize(int sizeInput, unsigned char* buf) {
	
	if(sizeInput >= 0 && sizeInput <= 127) {
		*buf = (char)sizeInput;
		return 1;
	} else if(sizeInput > 127 && sizeInput <= 0x7fffffff) {
		buf[0] = sizeInput >> 24 | 0x80;
		buf[1] = sizeInput >> 16 & 0xff;
		buf[2] = sizeInput >> 8 & 0xff;
		buf[3] = sizeInput & 0xff;
		return 4;
	} else {
		return -1;
	}
}

int jMsgDecodeSize(unsigned char* buf, int* sizeLen) {
	if(*buf & 0x80) {
		buf[0] = buf[0] ^ 0x80;
		*sizeLen = 4;
		int val1 = buf[0] ? ((int) buf[0])  << 24 : 0;
		int val2 = buf[1] ? ((int) buf[1])  << 16 : 0;
		int val3 = buf[2] ? ((int) buf[2])  << 8 : 0;
		return val1 + val2 + val3 +  buf[3];
	} else {
		*sizeLen = 1;
		return buf[0];
	}
}