#ifndef JMSG_READER_H
#define JMSG_READER_H
#include <string>
class JMsgReader {
public:
	JMsgReader(unsigned char* buf, int len);
	int peekMessageTypeId(bool& isSuccess);
	int readMessageTypeId(bool& isSuccess);
	int readFieldId(bool& isSuccess);
	int readArrayLength(bool& isSuccess);
	int readInt(bool& isSuccess);

    int64_t readInt64(bool& isSuccess);
	bool readBool(bool& isSuccess);
	std::string readString(bool& isSuccess);
	double readDouble(bool& isSuccess);
	int readEncodedLen(bool& isSuccess);
	void skipLen(int len);
private:
	unsigned char* m_buf;
	int m_len;
	unsigned char* m_cur;
	unsigned char* m_endPtr;
};
#endif