#include "jmsg_reader.h"
#include "jmsg_util.h"
using namespace std;

JMsgReader::JMsgReader(unsigned char* buf, int len) {
	m_buf = buf;
	m_len = len;
	m_cur = m_buf;
	m_endPtr = m_buf + len;
}

int JMsgReader::peekMessageTypeId(bool& isSuccess) {
	int sizeLen = 0;
	int size = JMsgDecodeSize(m_cur, &sizeLen, m_endPtr);

	if(size < 0) {
		isSuccess = false;
		return -1;
	}

	return size;
}


int JMsgReader::readMessageTypeId(bool& isSuccess) {
	int sizeLen = 0;
	int size = JMsgDecodeSize(m_cur, &sizeLen, m_endPtr);

	if(size < 0) {
		isSuccess = false;
		return -1;
	}
	m_cur += sizeLen;
	return size;
}

int JMsgReader::readFieldId(bool& isSuccess) {
	int sizeLen = 0;
	int size = JMsgDecodeSize(m_cur, &sizeLen, m_endPtr);

	if(size < 0) {
		isSuccess = false;
		return -1;
	}
	isSuccess = true;
	m_cur += sizeLen;
	return size;
}

int JMsgReader::readArrayLength(bool& isSuccess) {
	int sizeLen = 0;
	int size = JMsgDecodeSize(m_cur, &sizeLen, m_endPtr);

	if(size < 0) {
		isSuccess = false;
		return -1;
	}

	isSuccess = true;
	m_cur += sizeLen;
	return size;
}

int JMsgReader::readInt(bool& isSuccess) {

	if(m_endPtr - m_cur < 4) {
		isSuccess = false;
		return -1;
	}
	int val1 = m_cur[0] ? ((int)m_cur[0]) << 24 : 0;
	int val2 = m_cur[1] ? ((int) m_cur[1]) << 16 : 0;
	int val3 = m_cur[2] ? ((int)m_cur[2]) << 8 : 0;
	int val4 =  m_cur[3];

	int ret = val1 + val2 + val3+ val4;
	m_cur += 4;
	isSuccess = true;
	return ret;
}

bool JMsgReader::readBool(bool& isSuccess) {

	if(m_endPtr - m_cur < 1) {
		isSuccess = false;
		return false;
	}
	isSuccess = true;
	bool ret = m_cur[0] > 0;
	m_cur += 1;
	return ret;
}

std::string JMsgReader::readString(bool& isSuccess) {
	int sizeLen = 0;
	int size = JMsgDecodeSize(m_cur, &sizeLen, m_endPtr);

	if(size < 0) {
		isSuccess = false;
		return "";
	}
	
	m_cur += sizeLen;

	if(m_endPtr - m_cur < size) {
		isSuccess = false;
		return "";
	}
	isSuccess = true;
	string ret;
	ret.append((char*)m_cur, size);
	m_cur += size;
	return ret;
}

double JMsgReader::readDouble(bool& isSuccess) {
	isSuccess = true;
	double ret = *(double*)m_cur;
	m_cur += sizeof(double);
	return ret;	
}

int JMsgReader::readEncodedLen(bool& isSuccess)
{
	return readFieldId(isSuccess);
}

void JMsgReader::skipLen( int len )
{
	m_cur += len;
}
