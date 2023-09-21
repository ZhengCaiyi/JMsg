#include "jmsg_type.h"
#include "jmsg_field.h"
using namespace std;

JMsgField* JMsgType::getFieldByName( const string& fieldName )
{
	auto iter = m_mapFields.find(fieldName);
	if(iter == m_mapFields.end()) {
		return nullptr;
	}
	return iter->second;
}
