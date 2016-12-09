#ifndef JMSG_PROTO_H
#define JMSG_PROTO_H
#include <vector>
#include <string>
#include <map>
class JMsgType;
class JMsgWriter;
class JMsgReader;
class JMsgField;
typedef bool (*JMsgProtoDecodeCallback)(JMsgField* type, JMsgReader* reader, void* args);
typedef bool (*JMsgProtoEncodeCallback)(JMsgField* type, JMsgWriter* writer, void* args);
class JMsgProto {
public:
	JMsgProto() {}
	static JMsgProto*  createProto(const std::string& idlString);
	bool encode(const std::string& typeName, JMsgWriter* writer, JMsgProtoEncodeCallback callback, void* args);
	bool decode(JMsgReader* reader, JMsgProtoDecodeCallback callback, void* args);
	std::vector<JMsgType*>& getAllTypes();
	JMsgType* getTypeByName(const std::string& name);
	JMsgType* getTypeById(int id);
private:
	//JMsgProto() {}
	std::vector<JMsgType*> m_vecTypes;
	std::map<std::string, int> m_mapNameToIndex;
	std::map<int, int> m_mapIdToIndex;
};

#endif