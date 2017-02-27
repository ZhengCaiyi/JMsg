#include "AddressInfo.h"
#include "jmsg.h"

static bool onAddressInfoDecode(JMsgProto* proto, JMsgField* field, JMsgReader* reader, void* args) {
   bool isSuccess = false;
   AddressInfo* value = (AddressInfo*)args;
   switch(field->m_id) {
   case 1: {
      value->street = reader->readString(isSuccess);
      break;
   }
   case 2: {
      value->number = reader->readInt(isSuccess);
      break;
   }
   default:
      break;
   }
   return isSuccess;
}

static bool onAddressInfoEncode(JMsgProto* proto, JMsgField* field, JMsgWriter* writer, void* args) {
   AddressInfo* value = (AddressInfo*)args;
   switch(field->m_id) {
   case 1: {
      writer->writeStringField(field, value->street);
      break;
   }
   case 2: {
      writer->writeIntField(field, value->number);
      break;
   }
   default:
      break;
   }
   return true;
}
void AddressInfo::encode(JMsgProto* proto, JMsgWriter* writer) {
   proto->encode(10, writer, onAddressInfoEncode, this);
}
bool AddressInfo::decode(JMsgProto* proto, JMsgReader* reader) {
   return proto->decode(reader, onAddressInfoDecode, this) == 10;
}
