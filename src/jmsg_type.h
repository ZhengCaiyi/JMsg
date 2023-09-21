#ifndef JMSG_TYPE_H
#define JMSG_TYPE_H
#include <string>
#include <vector>
#include <unordered_map>
#include "jmsg_field.h"
class JMsgType {
public:
	std::string m_typeName;
	std::vector<JMsgField*> m_vecFields;
	std::unordered_map<std::string, JMsgField*> m_mapFields;

	JMsgField* getFieldByName(const std::string& fieldName);
	~JMsgType() {
		for(size_t i = 0; i < m_vecFields.size(); i++) {
			JMsgField* field = m_vecFields[i];
			delete field;
		}
	}
};

#endif