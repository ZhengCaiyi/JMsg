#ifndef JMSG_ENCODEABLE_H
#define JMSG_ENCODEABLE_H
class JMsgProto;
class JMsgWriter;
class JMsgReader;
class IJMsgEncodeable {
public:
	virtual void encode(JMsgProto* proto, JMsgWriter* writer) {}
	virtual bool decode(JMsgProto* proto, JMsgReader* reader) { return true; }
	virtual void encodeJson(JMsgProto* proto, Json::Value& val){}
	virtual bool decodeJson(JMsgProto* proto, Json::Value& val) {return true;}
	int getMsgId() { return m_msgId; } 
protected:
	int m_msgId;
};
#endif
