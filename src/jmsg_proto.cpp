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

JMsgProto*  JMsgProto::createProto(const string& idlString, bool fixFieldLen) {
	JMsgProto* proto = new JMsgProto;
	proto->m_fixFieldLen = fixFieldLen;
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
		if (getFixFieldLen()) {
			JMsgWriter tempWriter;
			JMsgField* field = msgType->m_vecFields[i];
			callback(this, field, &tempWriter, args);
			writer->writeEncodedLength(tempWriter.getBufferLen());
			writer->appendBuffer(&tempWriter);
		}
		else {
			JMsgField* field = msgType->m_vecFields[i];
			callback(this, field, writer, args);
		}
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

		if (getFixFieldLen()) {
			JMsgWriter tempWriter;
			JMsgField* field = msgType->m_vecFields[i];
			callback(this, field, &tempWriter, args);
			writer->writeEncodedLength(tempWriter.getBufferLen());
			writer->appendBuffer(&tempWriter);
		}
		else {
			JMsgField* field = msgType->m_vecFields[i];
			callback(this, field, writer, args);
		}		
	
	}
	writer->writeFieldHeader(0);
	return true;
}

int JMsgProto::decode( JMsgReader* reader, JMsgProtoDecodeCallback callback, void* args) {
	bool isSuccess = false;
	int typeId = reader->readFieldId(isSuccess);

	if(!isSuccess) {
		return -1;
	}

	JMsgType* msgType = getTypeById(typeId);
	int fieldId = 0;
	if(!msgType) {
		return -1;
	}

	do {

		if (getFixFieldLen()) {
			int fieldLen = reader->readEncodedLen(isSuccess);
			if (fieldLen == 0) {
				break;
			}

			fieldId = reader->peekMessageTypeId(isSuccess);

			if (!isSuccess) {
				return -1;
			}

			JMsgField* field = msgType->getFieldById(fieldId);

			if (!field) {
				reader->skipLen(fieldLen);
				continue;
			}
			reader->readFieldId(isSuccess);
			if (!callback(this, field, reader, args)) {
				return -1;
			}
		}
		else {
			fieldId = reader->readFieldId(isSuccess);

			if (fieldId == 0) {
				break;
			}

			JMsgField* field = msgType->getFieldById(fieldId);

			if (!field) {
				return -1;
			}
			if (!callback(this, field, reader, args)) {
				return -1;
			}
		}
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
	bool isSuccess = false;
	int typeId = reader->readFieldId(isSuccess);
	JMsgType* msgType = getTypeById(typeId);
	int fieldId = 0;
	if(!msgType) {
		return;
	}
	result.append("{");
	int currentFieldCount = 0;

	do {
		int fieldLen = 0;
		JMsgField* field = NULL;
		if (getFixFieldLen()) {
			fieldLen = reader->readEncodedLen(isSuccess);
			if (!fieldLen) {
				break;
			}
		}

		if(getFixFieldLen()) {
			fieldId = reader->peekMessageTypeId(isSuccess);
		}
		else {
			fieldId = reader->readFieldId(isSuccess);
		}
		

		field = msgType->getFieldById(fieldId);


		if (getFixFieldLen()) {
			if (!field) {
				reader->skipLen(fieldLen);
				continue;
			}
			reader->readMessageTypeId(isSuccess);
		}
		else {
			if (!field) {
				break;
			}
		}


		if(currentFieldCount > 0) {
			result.append(",");
		}
		if(!field->m_isArray) {
			if(field->m_type == "int") {
				jMsgAppendFormatString(result, "\"%s\": %d", field->m_name.c_str(), reader->readInt(isSuccess));
			} else if(field->m_type == "double") {
				jMsgAppendFormatString(result, "\"%s\": %f", field->m_name.c_str(), reader->readDouble(isSuccess));
			}  else if(field->m_type == "string") {

				jMsgAppendFormatString(result, "\"%s\": \"%s\"", field->m_name.c_str(), reader->readString(isSuccess).c_str());
			} else if(field->m_type == "bool") {
				jMsgAppendFormatString(result, "\"%s\": %s", field->m_name.c_str(), reader->readBool(isSuccess) ? "true" : "false");
			} else {
				jMsgAppendFormatString(result, "\"%s\": ", field->m_name.c_str());
				toJson(reader, len, result);
			}
		} else {
			int arrayCount = reader->readArrayLength(isSuccess);
			jMsgAppendFormatString(result, "\"%s\": [", field->m_name.c_str());
			bool arrayItemWritten = false;
			for(int i = 0; i < arrayCount; i++) {
				if(arrayItemWritten) {
					jMsgAppendFormatString(result, ",");
				}
				if(field->m_type == "int") {
					jMsgAppendFormatString(result, "%d", reader->readInt(isSuccess));
				} else if(field->m_type == "double") {
					jMsgAppendFormatString(result, "%f", reader->readDouble(isSuccess));
				}  else if(field->m_type == "string") {
					jMsgAppendFormatString(result, "\"%s\"", reader->readString(isSuccess).c_str());
				} else if(field->m_type == "bool") {
					jMsgAppendFormatString(result, " %s", reader->readBool(isSuccess) ? "true" : "false");
				} else {
					toJson(reader, len, result);
				}
				arrayItemWritten = true;
			}
			jMsgAppendFormatString(result, "]");
		}

		currentFieldCount++;
	} while(true);
	result.append("}");
}
