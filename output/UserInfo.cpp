#include "UserInfo.h"
#include "jmsg.h"

static bool onUserInfoDecode(JMsgProto* proto, JMsgField* field, JMsgReader* reader, void* args) {
   bool isSuccess = false;
   UserInfo* value = (UserInfo*)args;
   switch(field->m_id) {
   case 1: {
      value->userName = reader->readString(isSuccess);
      break;
   }
   case 2: {
      value->password = reader->readString(isSuccess);
      break;
   }
   case 3: {
      int arrayLen = reader->readArrayLength(isSuccess);
      if(!isSuccess) break;
      for(int i = 0; i < arrayLen; i++) {
         AddressInfo item;
         isSuccess = item.decode(proto, reader);
         if(!isSuccess) break;
         value->addresses.push_back(item);
      }
      break;
   }
   case 4: {
      value->age = reader->readInt(isSuccess);
      break;
   }
   case 5: {
      value->sex = reader->readBool(isSuccess);
      break;
   }
   default:
      break;
   }
   return isSuccess;
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
      int arrayLen = (int)value->addresses.size();
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
   case 5: {
      writer->writeBoolField(field, value->sex);
      break;
   }
   default:
      break;
   }
   return true;
}
void UserInfo::encode(JMsgProto* proto, JMsgWriter* writer) {
   proto->encode(12, writer, onUserInfoEncode, this);
}
bool UserInfo::decode(JMsgProto* proto, JMsgReader* reader) {
   return proto->decode(reader, onUserInfoDecode, this) == 12;
}
