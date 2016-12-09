#ifndef JMSG_READER_H
#define JMSG_READER_H
#include <string>
class JMsgReader {
public:
	JMsgReader(unsigned char* buf, int len);
	int peekMessageTypeId();
	int readMessageTypeId();
	int readFieldId();
	int readArrayLength();
	int readInt();
	bool readBool();
	std::string readString();
	double readDouble();
private:
	unsigned char* m_buf;
	int m_len;
	unsigned char* m_cur;
};
#endif