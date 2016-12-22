#include "jmsg_proto.h"
#include "jmsg_writer.h"
#include "jmsg_reader.h"
#include "jmsg_type.h"
#include "jmsg_field.h"
#include "jmsg_idl_parse.h"
#include "jmsg_util.h"

using namespace std;

JMsgProto::~JMsgProto() {
	for(size_t i = 0; i < m_vecTypes.size(); i++) {
		delete m_vecTypes[i];
	}
}

JMsgProto*  JMsgProto::createProto(const string& idlString) {
	JMsgProto* proto = new JMsgProto;
	if(!jMsgIDLParse(idlString, proto->m_vecTypes)) {
		delete proto;
		return NULL;
	}


	for(size_t i = 0; i < proto->m_vecTypes.size(); i++) {
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
	for(size_t i = 0; i < msgType->m_vecFields.size(); i++) {
		JMsgField* field = msgType->m_vecFields[i];
		callback(this, field, writer, args);
	}

	// fields end with 0
	writer->writeFieldHeader(0);
	return true;
}

bool JMsgProto::encode(const std::string& typeName, JMsgWriter* writer, JMsgProtoEncodeCallback callback, void* args) {
	JMsgType* msgType = getTypeByName(typeName);
	if(!msgType) {
		return false;
	}
	writer->writeFieldHeader(msgType->m_id);
	for(size_t i = 0; i < msgType->m_vecFields.size(); i++) {
		JMsgField* field = msgType->m_vecFields[i];
		callback(this, field, writer, args);
	}
	writer->writeFieldHeader(0);
	return true;
}

int JMsgProto::decode( JMsgReader* reader, JMsgProtoDecodeCallback callback, void* args) {
	int typeId = reader->readFieldId();
	JMsgType* msgType = getTypeById(typeId);
	int fieldId = 0;
	if(!msgType) {
		return -1;
	}

	do {
		fieldId = reader->readFieldId();
		if(fieldId == 0) {
			
			break;
		}

		JMsgField* field = msgType->getFieldById(fieldId);

		if(!field) {
			break;
		}

		callback(this, field, reader, args);
	} while(true);
	return typeId;
}

JMsgType* JMsgProto::getTypeByName(const std::string& name) {
	map<string, size_t>::iterator iter = m_mapNameToIndex.find(name);
	if(iter == m_mapNameToIndex.end()) {
		return NULL;
	}
	return m_vecTypes[iter->second];
}

JMsgType* JMsgProto::getTypeById(int id) {
	map<int, size_t>::iterator iter = m_mapIdToIndex.find(id);
	if(iter == m_mapIdToIndex.end()) {
		return NULL;
	}

	return m_vecTypes[iter->second]; 
}

static string getQuoteString(const string& content) {
	string ret;
	for(size_t i = 0; i < content.size(); i++) {
		if(content[i] == '\n') {
			ret.append("\\n");
		} else if(content[i] == '\r') {
			ret.append("\\r");
		} else if(content[i] == '\t') {
			ret.append("\\t");
		} else {
			ret.append((char*)content.c_str()+ i);
		}
	}
	return ret;
}

void JMsgProto::toJson(JMsgReader* reader, int len, string& result)
{
	int typeId = reader->readFieldId();
	JMsgType* msgType = getTypeById(typeId);
	int fieldId = 0;
	if(!msgType) {
		return;
	}
	result.append("{");
	int currentFieldCount = 0;

	do {
		fieldId = reader->readFieldId();
		if(fieldId == 0) {		
			break;
		}

		JMsgField* field = msgType->getFieldById(fieldId);

		if(!field) {
			break;
		}

		if(currentFieldCount > 0) {
			result.append(",");
		}
		if(!field->m_isArray) {
			if(field->m_type == "int") {
				jMsgAppendFormatString(result, "\"%s\": %d", field->m_name.c_str(), reader->readInt());
			} else if(field->m_type == "double") {
				jMsgAppendFormatString(result, "\"%s\": %f", field->m_name.c_str(), reader->readDouble());
			}  else if(field->m_type == "string") {

				jMsgAppendFormatString(result, "\"%s\": \"%s\"", field->m_name.c_str(), reader->readString().c_str());
			} else if(field->m_type == "bool") {
				jMsgAppendFormatString(result, "\"%s\": %s", field->m_name.c_str(), reader->readBool() ? "true" : "false");
			} else {
				jMsgAppendFormatString(result, "\%s\": ", field->m_name.c_str());
				toJson(reader, len, result);
			}
		} else {
			int arrayCount = reader->readArrayLength();
			jMsgAppendFormatString(result, "\"%s\": [", field->m_name.c_str());
			bool arrayItemWritten = 0;
			for(int i = 0; i < arrayCount; i++) {
				if(arrayItemWritten > 0) {
					jMsgAppendFormatString(result, ",");
				}
				if(field->m_type == "int") {
					jMsgAppendFormatString(result, "%d", field->m_name.c_str(), reader->readInt());
				} else if(field->m_type == "double") {
					jMsgAppendFormatString(result, "%f", field->m_name.c_str(), reader->readDouble());
				}  else if(field->m_type == "string") {
					jMsgAppendFormatString(result, "%s", field->m_name.c_str(), getQuoteString(reader->readString()).c_str());
				} else if(field->m_type == "bool") {
					jMsgAppendFormatString(result, " %s", field->m_name.c_str(), reader->readBool() ? "true" : "false");
				} else {
					toJson(reader, len, result);
				}
				arrayItemWritten++;
			}
			jMsgAppendFormatString(result, "]");
		}

		currentFieldCount++;
	} while(true);
	result.append("}");
}
