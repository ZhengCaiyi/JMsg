#ifndef JMSG_PROTO_H
#define JMSG_PROTO_H
#include "jmsg_config.h"
#include <vector>
#include <string>
#include <map>
#ifdef JMSG_SUPPORT_JSON
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#endif

class JMsgType;
class JMsgField;
class JMsgProto;
typedef bool (*JMsgProtoDecodeJsonCallback)(JMsgProto* proto, JMsgField* type, rapidjson::Value& reader, void* args);
typedef bool (*JMsgProtoEncodeJsonCallback)(JMsgProto* proto, JMsgField* type, rapidjson::Document& doc, rapidjson::Value& writer, void* args);

class JMsgProto {
public:
	JMsgProto() {}
	~JMsgProto();
	static JMsgProto*  createProto(const std::string& idlString);
	std::vector<JMsgType*>& getAllTypes() { return m_vecTypes; }
	JMsgType* getTypeByName(const std::string& name);
	JMsgType* getTypeById(int id);

#ifdef JMSG_SUPPORT_JSON
	bool encodeJson(const std::string& typeName, rapidjson::Document& doc, rapidjson::Value& obj, JMsgProtoEncodeJsonCallback callback, void* args);
	
	bool decodeJson(const std::string& typeName, rapidjson::Value& obj, JMsgProtoDecodeJsonCallback, void* args);
#endif

private:
	std::vector<JMsgType*> m_vecTypes;
	std::map<std::string, size_t> m_mapNameToIndex;
	std::map<int, size_t> m_mapIdToIndex;
};
#endif