#ifndef JMSG_PROTO_H
#define JMSG_PROTO_H
#include <vector>
#include <string>
#include <map>
class JMsgType;
class JMsgWriter;
class JMsgReader;
class JMsgField;
class JMsgProto;
typedef bool (*JMsgProtoDecodeCallback)(JMsgProto* proto, JMsgField* type, JMsgReader* reader, void* args);
typedef bool (*JMsgProtoEncodeCallback)(JMsgProto* proto, JMsgField* type, JMsgWriter* writer, void* args);
class JMsgProto {
public:
	JMsgProto() {}
	~JMsgProto();
	static JMsgProto*  createProto(const std::string& idlString);
	bool encode(int typeId, JMsgWriter* writer, JMsgProtoEncodeCallback callback, void* args);
	bool encode(const std::string& typeName, JMsgWriter* writer, JMsgProtoEncodeCallback callback, void* args);
	int decode(JMsgReader* reader, JMsgProtoDecodeCallback callback, void* args);
	void toJson(JMsgReader* reader, int len, std::string& result);
	std::vector<JMsgType*>& getAllTypes() {return m_vecTypes;}
	JMsgType* getTypeByName(const std::string& name);
	JMsgType* getTypeById(int id);
private:
	std::vector<JMsgType*> m_vecTypes;
	std::map<std::string, size_t> m_mapNameToIndex;
	std::map<int, size_t> m_mapIdToIndex;
};

#endif