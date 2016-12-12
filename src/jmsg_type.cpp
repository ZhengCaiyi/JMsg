#include "jmsg_type.h"
#include "jmsg_field.h"
JMsgField* JMsgType::getFieldById(int fieldId) {
	for(size_t i = 0; i < m_vecFields.size(); i++) {
		JMsgField* field = m_vecFields[i];

		if(field->m_id == fieldId) {
			return field;
		}
	}

	return NULL;
}