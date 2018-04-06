#ifndef JMSG_ENCODEABLE_H
#define JMSG_ENCODEABLE_H
#include "jmsg_config.h"
class JMsgProto;
class JMsgWriter;
class JMsgReader;
class IJMsgEncodeable {
public:
	virtual void encode(JMsgWriter* writer) {}
	virtual bool decode(JMsgReader* reader) {return true;}

#ifdef JMSG_SUPPORT_JSON
	virtual void encodeJson(Json::Value& val) {}
	virtual bool decodeJson(Json::Value& val) {return true;}
#endif // JMSG_SUPPORT_JSON
	int getMsgId() { return m_msgId; } 
protected:
	int m_msgId;
};
#endif // JMSG_ENCODEABLE_H
