#include "jmsg_writer.h"
#include "jmsg_util.h"
#include <string>
void JMsgWriter::writeFieldId(int len) {
	unsigned char lenBuffer[4] = {};
	int encodedLen = jMsgEcodeSize(len, lenBuffer);
	m_buffer.append((char*)lenBuffer, encodedLen);
}

void JMsgWriter::writeArrayLength(int len) {
	unsigned char lenBuffer[4] = {};
	int encodedLen = jMsgEcodeSize(len, lenBuffer);
	m_buffer.append((char*)lenBuffer, encodedLen);
}

void JMsgWriter::writeString(const std::string& str) {
	unsigned char lenBuffer[4] = {};
	int encodedLen = jMsgEcodeSize(str.size(), lenBuffer);
	m_buffer.append((char*)lenBuffer, encodedLen);
	m_buffer.append(str.c_str(), str.size());
}

void JMsgWriter::writeInt(const int value) {
	unsigned char buf[4] = {};
	buf[0] = (value << 24) & 0xff;
	buf[1] = (value << 16) & 0xff;
	buf[2] = (value << 0) & 0xff;
	buf[3] = value & 0xff;

	m_buffer.append((char*)buf, 4);
}

void JMsgWriter::writeDouble(const double value) {
	m_buffer.append((char*)&(value), sizeof(double));
}

void JMsgWriter::writeBool(const bool value) {
	char val = value ? 1 : 0;
	m_buffer.append(&val, 1);
}


char* JMsgWriter::getBuffer() {
	return (char*)m_buffer.c_str();
}

int JMsgWriter::getBufferLen() {
	return m_buffer.size();
}