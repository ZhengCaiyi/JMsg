#ifndef JMSG_WRITER_H
#define JMSG_WRITER_H
#include <string>
class JMsgField;
class JMsgWriter {
public:
	void writeFieldHeader(JMsgField* field);
	void writeFieldHeader(int fieldId);
	void writeArrayHeader(JMsgField* field, int length);
	void writeStringField(JMsgField* field, const std::string& str);
	void writeIntField(JMsgField* field, const int value);
	void writeDoubleField(JMsgField* field, const double value);
	void writeBoolField(JMsgField* field, const bool value);

	void writeString(const std::string& str);
	void writeInt(const int value);
	void writeDouble(const double value);
	void writeBool(const bool value);
	char* getBuffer();
	int getBufferLen();
private:
	void writeEncodedLength(int length);
	std::string m_buffer;
};
#endif
