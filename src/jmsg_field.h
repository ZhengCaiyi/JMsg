#ifndef JMSG_FIELD_H
#define JMSG_FIELD_H
#include <string>
class JMsgField {
public:
	std::string m_name;
	std::string m_type;
	bool m_isArray;
	int m_id;
	int m_typeId;
};
#endif
