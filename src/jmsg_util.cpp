#include <stdio.h>
#include <stdarg.h>
#include "jmsg_util.h"
#include "jmsg_encodeable.h"
#include "rapidjson/document.h"

#include "rapidjson/rapidjson.h"

#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
using namespace std;
bool JMsgIsDigit(char c) {
	return c >= '0' && c <= '9';
}

bool JMsgIsAlpha(char c) {
	return c >= 'A' && c <= 'z';
}

bool JMsgIsUnderLine(char c) {
	return c == '_';
}

bool JMsgIsEmptyChar(char c) {
	return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

bool JMsgGetFileString(const std::string& path, string& data) {
	FILE* file;
	file = fopen(path.c_str(), "r");

	if (!file) {
		return false;
	}

	char readBuffer[4096] = {};

	while (!feof(file)) {
		size_t len = fread(readBuffer, 1, sizeof(readBuffer), file);
		data.append(readBuffer, len);
	}
	fclose(file);
	return true;
}

int JMsgEcodeSize(int sizeInput, unsigned char* buf) {
	
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

int JMsgDecodeSize(unsigned char* buf, int* sizeLen, unsigned char* endPtr) {
	if(*buf & 0x80) {

		if(endPtr - buf < 4) {
			return -1;
		}
		unsigned char firstChar =  buf[0] ^ 0x80;
		*sizeLen = 4;
		int val1 = firstChar ? ((int) firstChar)  << 24 : 0;
		int val2 = buf[1] ? ((int) buf[1])  << 16 : 0;
		int val3 = buf[2] ? ((int) buf[2])  << 8 : 0;
		return val1 + val2 + val3 +  buf[3];
	} else {

		if(endPtr - buf < 1) {
			return -1;
		}
		*sizeLen = 1;
		return buf[0];
	}
}


std::string JMsgGetFormatString(const char * format, ...) {
	std::string res;
	char buf[4096] = {};
	char* backup_buf = NULL;
	int cur_buf_len = sizeof(buf);
	va_list ap;
	va_start(ap, format);
	int len = vsnprintf(buf, cur_buf_len, format, ap);
	va_end(ap);
	while(len >= cur_buf_len) {
		cur_buf_len *= 2;
		if(backup_buf) {
			free(backup_buf);
		}

		backup_buf = (char*)calloc(1, cur_buf_len);
		va_start(ap, format);
		len = vsnprintf(backup_buf, cur_buf_len, format, ap);
		va_end(ap);
	}


	if(backup_buf) {
		res =backup_buf;
		free(backup_buf);
	} else {
		res = buf;
	}
	return res;
}

void JMsgAppendFormatString(std::string& data, const char* format, ...) {
	char buf[4096] = {};
	char* backup_buf = NULL;
	int cur_buf_len = sizeof(buf);
	va_list ap;
	va_start(ap, format);
	int len = vsnprintf(buf, cur_buf_len, format, ap);
	va_end(ap);
	while(len >= cur_buf_len) {
		cur_buf_len *= 2;
		if(backup_buf) {
			free(backup_buf);
		}

		backup_buf = (char*)calloc(1, cur_buf_len);
		va_start(ap, format);
		len = vsnprintf(backup_buf, cur_buf_len, format, ap);
		va_end(ap);
	}


	if(backup_buf) {
		data.append(backup_buf);
		free(backup_buf);
	} else {
		data.append(buf);
	}  
}

std::string JMsgToJson(IJMsgEncodeable & msg)
{
    rapidjson::Document doc;
    doc.SetObject();
    msg.encodeJson(doc, doc);

    rapidjson::StringBuffer buffer;
    buffer.Clear();
    rapidjson::Writer<rapidjson::StringBuffer> bufferwriter(buffer);
    doc.Accept(bufferwriter);
    return buffer.GetString();
}

bool JMsgFromJson(const string & json, IJMsgEncodeable & msg)
{
    rapidjson::Document doc;
    doc.Parse(json.c_str());

    if (!doc.IsObject()) {
        return false;
    }
    auto obj = doc.GetObject();
    return msg.decodeJson(doc);
}

bool JMsgIsChangeLine( char c )
{
	return c == '\n' || c == '\r';    
}

std::string JMsgGetFileNameWithoutExt( const std::string& fullfileName )
{
	string fileName = JmsgGetFileName(fullfileName);
	size_t pos = fileName.rfind('.');

	if(pos <= 0) {
		return fileName;
	} else {
		return fileName.substr(0, pos);
	}
}



std::string JMsgGetFileExt( const std::string& fullfileName )
{
	string fileName = JmsgGetFileName(fullfileName);
	size_t pos = fileName.rfind('.');

	if(pos < 0) {
		return "";
	} else {
		return fileName.substr(pos);
	}
}


std::string JmsgGetFilePath( const std::string& fileName )
{
	size_t pos1 = fileName.rfind('/');
	size_t pos2 = fileName.rfind('\\');
	size_t pos = pos1 > pos2 ? pos1 : pos2;

	if(pos < 0) {
		return "";
	} else {
		return fileName.substr(0, pos + 1);
	}
}

std::string JmsgGetFileName( const std::string& fileName )
{
	size_t pos1 = fileName.rfind('/');
	size_t pos2 = fileName.rfind('\\');
	size_t pos = pos1 > pos2 ? pos1 : pos2;

	if(pos < 0) {
		return fileName;
	} else if(pos == fileName.size() - 1){
		return "";
	} else {
		return fileName.substr(pos + 1);
	}
}

std::string JMsgGetFirstCharBigerCase( const std::string& data )
{
	if(data == "") {
		return "";
	}
	string ret = data;
	ret[0] = toupper(ret[0]);
	return ret;
	
}
