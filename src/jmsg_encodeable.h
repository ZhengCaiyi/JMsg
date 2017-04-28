#ifndef JMSG_ENCODEABLE_H
#define JMSG_ENCODEABLE_H
class JMsgProto;
class JMsgWriter;
class IJMsgEncodeable {
public:
	virtual void encode(JMsgProto* proto, JMsgWriter* writer) = 0;
	virtual bool decode(JMsgProto* proto, JMsgReader* reader) = 0;
};
#endif
