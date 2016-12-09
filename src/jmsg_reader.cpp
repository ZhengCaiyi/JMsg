#include "jmsg_reader.h"
#include "jmsg_util.h"
using namespace std;

JMsgReader::JMsgReader(unsigned char* buf, int len) {
	m_buf = buf;
	m_len = len;
	m_cur = m_buf;
}

int JMsgReader::readMessageTypeId() {
	int sizeLen = 0;
	int size = jMsgDecodeSize(m_cur, &sizeLen);
	m_cur += sizeLen;
	return size;
}

int JMsgReader::readFieldId() {
	int sizeLen = 0;
	int size = jMsgDecodeSize(m_cur, &sizeLen);
	m_cur += sizeLen;
	return size;
}

int JMsgReader::readArrayLength() {
	int sizeLen = 0;
	int size = jMsgDecodeSize(m_cur, &sizeLen);
	m_cur += sizeLen;
	return size;
}

int JMsgReader::readInt() {
	return m_cur[0] << 24 + m_cur[1] << 16 + m_cur[2] <<8 + m_cur[3];
}

bool JMsgReader::readBool() {
	return m_cur[0] > 0;
}

std::string JMsgReader::readString() {
	int sizeLen = 0;
	int size = jMsgDecodeSize(m_cur, &sizeLen);
	m_cur += sizeLen;
	string ret;
	ret.append((char*)m_cur, size);
	m_cur += size;
	return ret;
}

double JMsgReader::readDouble() {
	double ret = *(double*)m_cur;
	m_cur += sizeof(double);
	return ret;
}