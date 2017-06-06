#ifndef JMSG_ENCODEABLE_H
#define JMSG_ENCODEABLE_H
class JMsgProto;
class JMsgWriter;
class JMsgReader;
class IJMsgEncodeable {
public:
	virtual void encode(JMsgProto* proto, JMsgWriter* writer) = 0;
	virtual bool decode(JMsgProto* proto, JMsgReader* reader) = 0;
	int getMsgId() { return m_msgId; }
protected:
	int m_msgId;
};
#endif
