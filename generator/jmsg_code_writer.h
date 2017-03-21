#ifndef JMSG_CODE_WRITER
#define JMSG_CODE_WRITER
#include <stdio.h>
#include <string>
/*
#if defined(__GNUC__)

#define PRINTF_FORMAT(format_param, dots_param) \

    __attribute__((format(printf, format_param, dots_param)))
#else

#define PRINTF_FORMAT(format_param, dots_param)

#endif
*/
class JMSGCodeWriter {
public:
	JMSGCodeWriter() {m_file = NULL;m_indent=0;}
	~JMSGCodeWriter() {if(m_file) fclose(m_file);}
	void addIndent();
	void removeIndent();
	void writeLine(const char* format...);
	bool open(const std::string& path);
	void write(const char* format,...);
private:
	FILE* m_file;
	int m_indent;
};
#endif
