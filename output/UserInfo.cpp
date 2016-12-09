#include "UserInfo.h"
#include "jmsg.h"

static bool onUserInfoDecode(JMsgProto* proto, JMsgField* field, JMsgReader* reader, void* args) {
   UserInfo* value = (UserInfo*)args;
   switch(field->m_id) {
   case 1: {
      value->userName = reader->readString();
      break;
   }
   case 2: {
      value->password = reader->readString();
      break;
   }
   case 3: {
      int arrayLen = reader->readArrayLength();
      for(int i = 0; i < arrayLen; i++) {
         AddressInfo item;
         item.decode(proto, reader);
         value->addresses.push_back(item);
      }
      break;
   }
   case 4: {
      value->age = reader->readInt();
      break;
   }
   default:
      break;
   }
}

static bool onUserInfoEncode(JMsgProto* proto, JMsgField* field, JMsgWriter* writer, void* args) {
   UserInfo* value = (UserInfo*)args;
   switch(field->m_id) {
   case 1: {
      writer->writeStringField(field, value->userName);
      break;
   }
   case 2: {
      writer->writeStringField(field, value->password);
      break;
   }
   case 3: {
      int arrayLen = value->addresses.size();
      writer->writeArrayHeader(field, arrayLen);
      for(int i = 0; i < arrayLen; i++) {
         value->addresses[i].encode(proto, writer);
      }
      break;
   }
   case 4: {
      writer->writeIntField(field, value->age);
      break;
   }
   default:
      break;
   }
}
void UserInfo::encode(JMsgProto* proto, JMsgWriter* writer) {
   proto->encode(12, writer, onUserInfoEncode, this);
}
void UserInfo::decode(JMsgProto* proto, JMsgReader* reader) {
   proto->decode(reader, onUserInfoDecode, this);
}
