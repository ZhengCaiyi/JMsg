#include "jmsg_proto.h"
#include "jmsg_writer.h"
#include "jmsg_reader.h"
#include "jmsg_type.h"
#include "jmsg_field.h"
#include "jmsg_idl_parse.h"

using namespace std;

JMsgProto::~JMsgProto() {
	for(int i = 0; i < m_vecTypes.size(); i++) {
		delete m_vecTypes[i];
	}
}

JMsgProto*  JMsgProto::createProto(const string& idlString) {
	JMsgProto* proto = new JMsgProto;
	if(!jMsgIDLParse(idlString, proto->m_vecTypes)) {
		delete proto;
		return NULL;
	}


	for(int i = 0; i < proto->m_vecTypes.size(); i++) {
		proto->m_mapNameToIndex[proto->m_vecTypes[i]->m_typeName] = i;
		proto->m_mapIdToIndex[proto->m_vecTypes[i]->m_id] = i;
	}
	return proto;
}

bool JMsgProto::encode(int typeId, JMsgWriter* writer, JMsgProtoEncodeCallback callback, void* args) {
	JMsgType* msgType = getTypeById(typeId);
	if(!msgType) {
		return false;
	}
	writer->writeFieldHeader(msgType->m_id);
	for(int i = 0; i < msgType->m_vecFields.size(); i++) {
		JMsgField* field = msgType->m_vecFields[i];
		callback(this, field, writer, args);
	}
	writer->writeFieldHeader(0);

}

bool JMsgProto::encode(const std::string& typeName, JMsgWriter* writer, JMsgProtoEncodeCallback callback, void* args) {
	JMsgType* msgType = getTypeByName(typeName);
	if(!msgType) {
		return false;
	}
	writer->writeFieldHeader(msgType->m_id);
	for(int i = 0; i < msgType->m_vecFields.size(); i++) {
		JMsgField* field = msgType->m_vecFields[i];
		callback(this, field, writer, args);
	}
	writer->writeFieldHeader(0);

}

bool JMsgProto::decode( JMsgReader* reader, JMsgProtoDecodeCallback callback, void* args) {
	int typeId = reader->readFieldId();
	printf("decode read type id=%d\n", typeId);
	JMsgType* msgType = getTypeById(typeId);
	int fieldId = 0;
	if(!msgType) {
		printf("empty msg type, typeId=%d\n", typeId);
		return false;
	}

	do {
		fieldId = reader->readFieldId();
		printf("decode read field id=%d\n", fieldId);
		if(fieldId == 0) {
			
			break;
		}

		JMsgField* field = msgType->getFieldById(fieldId);

		if(!field) {
			printf("field id=%d not found\n", fieldId);
			break;
		}

		callback(this, field, reader, args);
	} while(true);
	return true;
}

JMsgType* JMsgProto::getTypeByName(const std::string& name) {
	auto iter = m_mapNameToIndex.find(name);
	if(iter == m_mapNameToIndex.end()) {
		return NULL;
	}
	return m_vecTypes[iter->second];
}

JMsgType* JMsgProto::getTypeById(int id) {
	auto iter = m_mapIdToIndex.find(id);
	if(iter == m_mapIdToIndex.end()) {
		return NULL;
	}

	return m_vecTypes[iter->second]; 
}