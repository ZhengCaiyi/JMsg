#include "jmsg_type.h"
#include "jmsg_field.h"
using namespace std;

JMsgField* JMsgType::getFieldByName( const string& fieldName )
{
	for(size_t i = 0; i < m_vecFields.size(); i++) {
		JMsgField* field = m_vecFields[i];

		if(field->m_name == fieldName) {
			return field;
		}
	}

	return NULL;
}
