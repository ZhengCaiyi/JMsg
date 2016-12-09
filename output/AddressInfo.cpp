#include "AddressInfo.h"
#include "jmsg.h"

static void onAddressInfoDecode(JMsgProto* proto, JMsgField* field, JMsgReader* reader, void* args) {
   AddressInfo* value = (AddressInfo*)args;
   switch(field->m_id) {
   case 1: {
      value.street = reader->readString()
      break;
   }
   case 2: {
      value.number = reader->readInt()
      break;
   }
   default:
      break
   }
}

static void onAddressInfoEncode(JMsgProto* proto, JMsgField* field, JMsgWriter* writer, void* args) {
   AddressInfo* value = (AddressInfo*)args;
   switch(field->m_id) {
   case 1: {
      writer->writeStringField(field, value.street[i])
      break;
   }
   case 2: {
      writer->writeIntField(field, value.number[i])
      break;
   }
   default:
      break
   }
}
void AddressInfo::encode(JMsgProto* proto, JMsgWriter* writer) {
   proto->encode(10, writer, onAddressInfoEncode, this);
}
void AddressInfo::decode(JMsgProto* proto, JMsgReader* reader) {
   proto->decode(reader, onAddressInfoDecode, this);
}
