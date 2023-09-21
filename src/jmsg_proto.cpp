#include "jmsg_proto.h"
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

#ifdef JMSG_SUPPORT_JSON
bool JMsgProto::encodeJson( const std::string& typeName, rapidjson::Document& doc, rapidjson::Value& obj, JMsgProtoEncodeJsonCallback callback, void* args )
{
	JMsgType* msgType = this->getTypeByName(typeName);

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
    return true;
}

#endif