#ifndef JMSG_WRITER_H
#define JMSG_WRITER_H
#include <string>
class JMsgWriter {
public:
	void writeFieldId(int);
	void writeArrayLength(int);
	void writeString(const std::string& str);
	void writeInt(const int value);
	void writeDouble(const double value);
	void writeBool(const bool value);
	char* getBuffer();
	int getBufferLen();
private:
	std::string m_buffer;
};
#endif
