#ifndef JMSG_ENCODEABLE_H
#define JMSG_ENCODEABLE_H
#include "jmsg_config.h"
#include "rapidjson/document.h"
class JMsgProto;
class JMsgWriter;
class JMsgReader;

class IJMsgEncodeable {
public:
    virtual void encodeJson(rapidjson::Document&, rapidjson::Value& val) = 0;
    virtual bool decodeJson(rapidjson::Value& val) = 0;
	int getMsgId() { return m_msgId; } 
protected:
	int m_msgId;
};
#endif // JMSG_ENCODEABLE_H
