#include "jmsg_reader.h"
#include "jmsg_util.h"
using namespace std;

JMsgReader::JMsgReader(unsigned char* buf, int len) {
	m_buf = buf;
	m_len = len;
	m_cur = m_buf;
}

int JMsgReader::peekMessageTypeId() {
	int sizeLen = 0;
	int size = jMsgDecodeSize(m_cur, &sizeLen);
	return size;
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
	int val1 = m_cur[0] ? ((int)m_cur[0]) << 24 : 0;
	int val2 = m_cur[1] ? ((int) m_cur[1]) << 16 : 0;
	int val3 = m_cur[2] ? ((int)m_cur[2]) << 8 : 0;
	int val4 =  m_cur[3];

	int ret = val1 + val2 + val3+ val4;
	m_cur += 4;
	return ret;
}

bool JMsgReader::readBool() {
	bool ret = m_cur[0] > 0;
	ret += 1;
	return ret;
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