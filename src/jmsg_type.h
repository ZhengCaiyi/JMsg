#ifndef JMSG_TYPE_H
#define JMSG_TYPE_H
#include <string>
#include <vector>
class JMsgField;
class JMsgType {
public:
	int m_id;
	std::string m_typeName;
	std::vector<JMsgField*> m_vecFields;

	JMsgField* getFieldById(int fieldId);
};

#endif