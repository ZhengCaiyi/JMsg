#include <stdarg.h>
#include <string>
#include "jmsg_code_writer.h"
#define TABSIZE 3
using namespace std;
bool JMSGCodeWriter::open(const string& path) {
	m_file = fopen(path.c_str(), "w");

	return m_file ? true : false;
}
void JMSGCodeWriter::addIndent() {
	m_indent ++;
}
void JMSGCodeWriter::removeIndent() {
	if(m_indent > 0) {
		m_indent --;
	}
}
void JMSGCodeWriter::writeLine(const char* format,...) {
	for(int i = 0; i < m_indent * TABSIZE; i++) {
		fprintf(m_file, " ");
	}

	va_list ap;
  	va_start(ap, format);
  	vfprintf(m_file, format, ap);
  	va_end(ap);
  	fprintf(m_file, "\n");
}