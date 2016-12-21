#include "jmsg_writer.h"
#include "jmsg_util.h"
#include "jmsg_field.h"
#include <string>

void JMsgWriter::writeFieldHeader(JMsgField* field) {
	writeEncodedLength(field->m_id);
}
void JMsgWriter::writeFieldHeader(int fieldId) {
	writeEncodedLength(fieldId);
}

void JMsgWriter::writeArrayHeader(JMsgField* field, int length) {
	writeEncodedLength(field->m_id);
	writeEncodedLength(length);
}

void JMsgWriter::writeStringField(JMsgField* field, const std::string& str) {
	writeEncodedLength(field->m_id);
	writeString(str);
}

void JMsgWriter::writeIntField(JMsgField* field, const int value) {
	writeEncodedLength(field->m_id);
	writeInt(value);
}

void JMsgWriter::writeDoubleField(JMsgField* field, const double value) {
	writeEncodedLength(field->m_id);
	writeDouble(value);
}

void JMsgWriter::writeBoolField(JMsgField* field, const bool value) {
	writeEncodedLength(field->m_id);
	writeBool(value);
}


void JMsgWriter::writeString(const std::string& str) {
	unsigned char lenBuffer[4] = {};
	int encodedLen = jMsgEcodeSize((int)str.size(), lenBuffer);
	m_buffer.append((char*)lenBuffer, encodedLen);
	m_buffer.append(str.c_str(), str.size());
}

void JMsgWriter::writeInt(const int value) {
	unsigned char buf[4] = {};
	buf[0] = (value >> 24) & 0xff;
	buf[1] = (value >> 16) & 0xff;
	buf[2] = (value >> 8) & 0xff;
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
	return (int)m_buffer.size();
}

void JMsgWriter::writeEncodedLength(int len) {
	unsigned char lenBuffer[4] = {};
	int encodedLen = jMsgEcodeSize(len, lenBuffer);
	m_buffer.append((char*)lenBuffer, encodedLen);
}