#ifndef JMSG_TYPE_H
#define JMSG_TYPE_H
#include <string>
#include <vector>
#include "jmsg_field.h"
class JMsgType {
public:
	int m_id;
	std::string m_typeName;
	std::vector<JMsgField*> m_vecFields;

	JMsgField* getFieldById(int fieldId);

	~JMsgType() {
		for(int i = 0; i < m_vecFields.size(); i++) {
			JMsgField* field = m_vecFields[i];
			delete field;
		}
	}
};

#endif