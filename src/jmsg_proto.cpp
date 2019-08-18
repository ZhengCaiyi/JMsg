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
		fieldLen = reader->readEncodedLen(isSuccess);
		if (!fieldLen) {
			break;
		}
			fieldId = reader->peekMessageTypeId(isSuccess);
		

		field = msgType->getFieldById(fieldId);


		if (!field) {
			reader->skipLen(fieldLen);
			continue;
		}
		reader->readMessageTypeId(isSuccess);


		if(currentFieldCount > 0) {
			result.append(",");
		}
		if(!field->m_isArray) {
			if(field->m_type == "int") {
				JMsgAppendFormatString(result, "\"%s\": %d", field->m_name.c_str(), reader->readInt(isSuccess));
			} else if(field->m_type == "double") {
				JMsgAppendFormatString(result, "\"%s\": %f", field->m_name.c_str(), reader->readDouble(isSuccess));
			}  else if(field->m_type == "string") {

				JMsgAppendFormatString(result, "\"%s\": \"%s\"", field->m_name.c_str(), reader->readString(isSuccess).c_str());
			} else if(field->m_type == "bool") {
				JMsgAppendFormatString(result, "\"%s\": %s", field->m_name.c_str(), reader->readBool(isSuccess) ? "true" : "false");
			} else {
				JMsgAppendFormatString(result, "\"%s\": ", field->m_name.c_str());
				toJson(reader, len, result);
			}
		} else {
			int arrayCount = reader->readArrayLength(isSuccess);
			JMsgAppendFormatString(result, "\"%s\": [", field->m_name.c_str());
			bool arrayItemWritten = false;
			for(int i = 0; i < arrayCount; i++) {
				if(arrayItemWritten) {
					JMsgAppendFormatString(result, ",");
				}
				if(field->m_type == "int") {
					JMsgAppendFormatString(result, "%d", reader->readInt(isSuccess));
				} else if(field->m_type == "double") {
					JMsgAppendFormatString(result, "%f", reader->readDouble(isSuccess));
				}  else if(field->m_type == "string") {
					JMsgAppendFormatString(result, "\"%s\"", reader->readString(isSuccess).c_str());
				} else if(field->m_type == "bool") {
					JMsgAppendFormatString(result, " %s", reader->readBool(isSuccess) ? "true" : "false");
				} else {
					toJson(reader, len, result);
				}
				arrayItemWritten = true;
			}
			JMsgAppendFormatString(result, "]");
		}

		currentFieldCount++;
	} while(true);
	result.append("}");
}

#ifdef JMSG_SUPPORT_JSON
bool JMsgProto::encodeJson( int typeId, rapidjson::Document& doc, rapidjson::Value& obj, JMsgProtoEncodeJsonCallback callback, void* args )
{
	JMsgType* msgType = this->getTypeById(typeId);

	if(!msgType) {
		return false;
	}

	vector<JMsgField*>& fields = msgType->m_vecFields;

	for(size_t i = 0; i < fields.size(); i++) {
		JMsgField* field = fields[i];

		callback(this, field, doc, obj, args);
	}
	return true;
}

bool JMsgProto::decodeJson( const string& typeName,  rapidjson::Value& obj, JMsgProtoDecodeJsonCallback callback, void* args )
{	
	JMsgType* msgType = this->getTypeByName(typeName);

	if(!msgType) {
		return false;
	}
    
    for (auto memIter = obj.MemberBegin(); memIter != obj.MemberEnd(); memIter++) {
        auto key = memIter->name.GetString();
        JMsgField* field = msgType->getFieldByName(key);
        if (!field) {
            continue;
        }
        callback(this, field, memIter->value, args);
    }

    /*
	Json::Value::Members mem = obj.getMemberNames(); 
	for(size_t i = 0;i < mem.size(); i++) {
		string& key = mem[i];
		
		JMsgField* field = msgType->getFieldByName(key);
		if(!field) {
			continue;
		}		
		callback(this, field, obj, args);
	}
    */
    return true;
}

bool JMsgProto::decodeJson( int typeId, rapidjson::Value& obj, JMsgProtoDecodeJsonCallback callback, void* args )
{	
	JMsgType* msgType = this->getTypeById(typeId);

	if(!msgType) {
		return false;
	}

    /*
	Json::Value::Members mem = obj.getMemberNames(); 
	for(size_t i = 0;i < mem.size(); i++) {
		string& key = mem[i];

		JMsgField* field = msgType->getFieldByName(key);
		if(!field) {
			continue;
		}		
		callback(this, field, obj[key], args);
	}*/
    for (auto memIter = obj.MemberBegin(); memIter != obj.MemberEnd(); memIter++) {
        auto key = memIter->name.GetString();
        JMsgField* field = msgType->getFieldByName(key);
        if (!field) {
            continue;
        }
        callback(this, field, memIter->value, args);
    }
	return true;
}
#endif