#ifndef JMSG_ENCODEABLE_H
#define JMSG_ENCODEABLE_H
#include "jmsg_config.h"
class JMsgProto;
class JMsgWriter;
class JMsgReader;
class IJMsgEncodeable {
public:
	virtual void encode(JMsgProto* proto, JMsgWriter* writer) = 0;
	virtual bool decode(JMsgProto* proto, JMsgReader* reader) = 0;

#ifdef JMSG_SUPPORT_JSON
	virtual void encodeJson(JMsgProto* proto, Json::Value& val) = 0;
	virtual bool decodeJson(JMsgProto* proto, Json::Value& val) = 0;
#endif // JMSG_SUPPORT_JSON
	int getMsgId() { return m_msgId; } 
protected:
	int m_msgId;
};
#endif // JMSG_ENCODEABLE_H
