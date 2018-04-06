#include "userInfo.h"
#include "jmsg.h"
#include "json/value.h"
static const unsigned char s_protoString []= {
   0x50,0x65,0x72,0x73,0x6f,0x6e,0x20,0x3d,0x20,0x31,
   0x20,0x7b,0x0d,0x0a,0x20,0x20,0x20,0x20,0x6e,0x61,
   0x6d,0x65,0x20,0x3a,0x20,0x73,0x74,0x72,0x69,0x6e,
   0x67,0x20,0x3d,0x20,0x31,0x0d,0x0a,0x20,0x20,0x20,
   0x20,0x69,0x64,0x20,0x3a,0x20,0x69,0x6e,0x74,0x20,
   0x3d,0x20,0x32,0x0d,0x0a,0x20,0x20,0x20,0x20,0x65,
   0x6d,0x61,0x69,0x6c,0x20,0x3a,0x20,0x73,0x74,0x72,
   0x69,0x6e,0x67,0x20,0x3d,0x20,0x33,0x0d,0x0a,0x20,
   0x20,0x20,0x20,0x70,0x68,0x6f,0x6e,0x65,0x20,0x3a,
   0x20,0x5b,0x5d,0x50,0x68,0x6f,0x6e,0x65,0x4e,0x75,
   0x6d,0x62,0x65,0x72,0x20,0x3d,0x20,0x34,0x0d,0x0a,
   0x7d,0x0d,0x0a,0x0d,0x0a,0x50,0x68,0x6f,0x6e,0x65,
   0x4e,0x75,0x6d,0x62,0x65,0x72,0x20,0x3d,0x20,0x32,
   0x20,0x7b,0x0d,0x0a,0x20,0x20,0x20,0x20,0x6e,0x75,
   0x6d,0x62,0x65,0x72,0x20,0x3a,0x20,0x73,0x74,0x72,
   0x69,0x6e,0x67,0x20,0x3d,0x20,0x31,0x0d,0x0a,0x20,
   0x20,0x20,0x20,0x74,0x79,0x70,0x65,0x20,0x3a,0x20,
   0x69,0x6e,0x74,0x20,0x3d,0x20,0x32,0x0d,0x0a,0x7d,
   0x0d,0x0a,0x0d,0x0a,0x41,0x64,0x64,0x72,0x65,0x73,
   0x73,0x42,0x6f,0x6f,0x6b,0x20,0x3d,0x20,0x33,0x20,
   0x7b,0x0d,0x0a,0x20,0x20,0x20,0x20,0x70,0x65,0x72,
   0x73,0x6f,0x6e,0x20,0x3a,0x20,0x5b,0x5d,0x50,0x65,
   0x72,0x73,0x6f,0x6e,0x20,0x3d,0x20,0x31,0x0d,0x0a,
   0x7d,0x00
};

JMsgProto* userInfoCreateProto() { return JMsgProto::createProto((char*)s_protoString); }
static JMsgProto* g_proto = NULL;
void userInfoInit(){
   g_proto = userInfoCreateProto();
}
void userInfoFini(){
   if(g_proto) {
      delete g_proto;
      g_proto = NULL;
   }
}

JMsgProto* userInfoGetProto() { return g_proto; }

PhoneNumber::PhoneNumber() {
   m_msgId = 2;
   type = 0;
}

static bool onPhoneNumberDecode(JMsgProto* proto, JMsgField* field, JMsgReader* reader, void* args) {
   bool isSuccess = false;
   PhoneNumber* value = (PhoneNumber*)args;
   switch(field->m_id) {
   case 1: {
      value->number = reader->readString(isSuccess);
      break;
   }
   case 2: {
      value->type = reader->readInt(isSuccess);
      break;
   }
   default:
      break;
   }
   return isSuccess;
}

static bool onPhoneNumberEncode(JMsgProto* proto, JMsgField* field, JMsgWriter* writer, void* args) {
   PhoneNumber* value = (PhoneNumber*)args;
   switch(field->m_id) {
   case 1: {
      writer->writeStringField(field, value->number);
      break;
   }
   case 2: {
      writer->writeIntField(field, value->type);
      break;
   }
   default:
      break;
   }
   return true;
}

void PhoneNumber::encode(JMsgWriter* writer) {
   g_proto->encode(2, writer, onPhoneNumberEncode, this);
}

bool PhoneNumber::decode(JMsgReader* reader) {
   return g_proto->decode(reader, onPhoneNumberDecode, this) == 2;
}

static bool onPhoneNumberDecodeJson(JMsgProto* proto, JMsgField* field, Json::Value& jsonValue, void* args) {
   bool isSuccess = false;
   PhoneNumber* value = (PhoneNumber*)args;
   switch(field->m_id) {
   case 1: {
      value->number = jsonValue.isString() ? jsonValue.asString() : "";
      break;
   }
   case 2: {
      value->type = jsonValue.isInt() ? jsonValue.asInt() : 0;
      break;
   }
   default:
      break;
   }
   return isSuccess;
}

static bool onPhoneNumberEncodeJson(JMsgProto* proto, JMsgField* field, Json::Value& jsonValue, void* args) {
   PhoneNumber* value = (PhoneNumber*)args;
   switch(field->m_id) {
   case 1: {
      jsonValue["number"] = value->number;
      break;
   }
   case 2: {
      jsonValue["type"] = value->type;
      break;
   }
   default:
      break;
   }
   return true;
}

void PhoneNumber::encodeJson( Json::Value& writer) {
   g_proto->encodeJson(2, writer, onPhoneNumberEncodeJson, this);
}

bool PhoneNumber::decodeJson( Json::Value& reader) {
   return g_proto->decodeJson(2, reader, onPhoneNumberDecodeJson, this);
}
Person::Person() {
   m_msgId = 1;
   id = 0;
}

static bool onPersonDecode(JMsgProto* proto, JMsgField* field, JMsgReader* reader, void* args) {
   bool isSuccess = false;
   Person* value = (Person*)args;
   switch(field->m_id) {
   case 1: {
      value->name = reader->readString(isSuccess);
      break;
   }
   case 2: {
      value->id = reader->readInt(isSuccess);
      break;
   }
   case 3: {
      value->email = reader->readString(isSuccess);
      break;
   }
   case 4: {
      int arrayLen = reader->readArrayLength(isSuccess);
      if(!isSuccess) break;
      for(int i = 0; i < arrayLen; i++) {
         PhoneNumber item;
         isSuccess = item.decode(reader);
         if(!isSuccess) break;
         value->phone.push_back(item);
      }
      break;
   }
   default:
      break;
   }
   return isSuccess;
}

static bool onPersonEncode(JMsgProto* proto, JMsgField* field, JMsgWriter* writer, void* args) {
   Person* value = (Person*)args;
   switch(field->m_id) {
   case 1: {
      writer->writeStringField(field, value->name);
      break;
   }
   case 2: {
      writer->writeIntField(field, value->id);
      break;
   }
   case 3: {
      writer->writeStringField(field, value->email);
      break;
   }
   case 4: {
      int arrayLen = (int)value->phone.size();
      writer->writeArrayHeader(field, arrayLen);
      for(int i = 0; i < arrayLen; i++) {
         value->phone[i].encode(writer);
      }
      break;
   }
   default:
      break;
   }
   return true;
}

void Person::encode(JMsgWriter* writer) {
   g_proto->encode(1, writer, onPersonEncode, this);
}

bool Person::decode(JMsgReader* reader) {
   return g_proto->decode(reader, onPersonDecode, this) == 1;
}

static bool onPersonDecodeJson(JMsgProto* proto, JMsgField* field, Json::Value& jsonValue, void* args) {
   bool isSuccess = false;
   Person* value = (Person*)args;
   switch(field->m_id) {
   case 1: {
      value->name = jsonValue.isString() ? jsonValue.asString() : "";
      break;
   }
   case 2: {
      value->id = jsonValue.isInt() ? jsonValue.asInt() : 0;
      break;
   }
   case 3: {
      value->email = jsonValue.isString() ? jsonValue.asString() : "";
      break;
   }
   case 4: {
      for(size_t i = 0; i < jsonValue.size(); i++) {
         PhoneNumber item;
         isSuccess = item.decodeJson(jsonValue[(int)i]);
         if(!isSuccess) break;
         value->phone.push_back(item);
      }
      break;
   }
   default:
      break;
   }
   return isSuccess;
}

static bool onPersonEncodeJson(JMsgProto* proto, JMsgField* field, Json::Value& jsonValue, void* args) {
   Person* value = (Person*)args;
   switch(field->m_id) {
   case 1: {
      jsonValue["name"] = value->name;
      break;
   }
   case 2: {
      jsonValue["id"] = value->id;
      break;
   }
   case 3: {
      jsonValue["email"] = value->email;
      break;
   }
   case 4: {
      Json::Value arrayValue;
      arrayValue.resize(0);
      for(size_t i = 0; i < value->phone.size(); i++) {
         Json::Value itemValue;
         value->phone[i].encodeJson(itemValue);
         arrayValue.append(itemValue);
      }
      jsonValue["phone"] = arrayValue;
      break;
   }
   default:
      break;
   }
   return true;
}

void Person::encodeJson( Json::Value& writer) {
   g_proto->encodeJson(1, writer, onPersonEncodeJson, this);
}

bool Person::decodeJson( Json::Value& reader) {
   return g_proto->decodeJson(1, reader, onPersonDecodeJson, this);
}
AddressBook::AddressBook() {
   m_msgId = 3;
}

static bool onAddressBookDecode(JMsgProto* proto, JMsgField* field, JMsgReader* reader, void* args) {
   bool isSuccess = false;
   AddressBook* value = (AddressBook*)args;
   switch(field->m_id) {
   case 1: {
      int arrayLen = reader->readArrayLength(isSuccess);
      if(!isSuccess) break;
      for(int i = 0; i < arrayLen; i++) {
         Person item;
         isSuccess = item.decode(reader);
         if(!isSuccess) break;
         value->person.push_back(item);
      }
      break;
   }
   default:
      break;
   }
   return isSuccess;
}

static bool onAddressBookEncode(JMsgProto* proto, JMsgField* field, JMsgWriter* writer, void* args) {
   AddressBook* value = (AddressBook*)args;
   switch(field->m_id) {
   case 1: {
      int arrayLen = (int)value->person.size();
      writer->writeArrayHeader(field, arrayLen);
      for(int i = 0; i < arrayLen; i++) {
         value->person[i].encode(writer);
      }
      break;
   }
   default:
      break;
   }
   return true;
}

void AddressBook::encode(JMsgWriter* writer) {
   g_proto->encode(3, writer, onAddressBookEncode, this);
}

bool AddressBook::decode(JMsgReader* reader) {
   return g_proto->decode(reader, onAddressBookDecode, this) == 3;
}

static bool onAddressBookDecodeJson(JMsgProto* proto, JMsgField* field, Json::Value& jsonValue, void* args) {
   bool isSuccess = false;
   AddressBook* value = (AddressBook*)args;
   switch(field->m_id) {
   case 1: {
      for(size_t i = 0; i < jsonValue.size(); i++) {
         Person item;
         isSuccess = item.decodeJson(jsonValue[(int)i]);
         if(!isSuccess) break;
         value->person.push_back(item);
      }
      break;
   }
   default:
      break;
   }
   return isSuccess;
}

static bool onAddressBookEncodeJson(JMsgProto* proto, JMsgField* field, Json::Value& jsonValue, void* args) {
   AddressBook* value = (AddressBook*)args;
   switch(field->m_id) {
   case 1: {
      Json::Value arrayValue;
      arrayValue.resize(0);
      for(size_t i = 0; i < value->person.size(); i++) {
         Json::Value itemValue;
         value->person[i].encodeJson(itemValue);
         arrayValue.append(itemValue);
      }
      jsonValue["person"] = arrayValue;
      break;
   }
   default:
      break;
   }
   return true;
}

void AddressBook::encodeJson( Json::Value& writer) {
   g_proto->encodeJson(3, writer, onAddressBookEncodeJson, this);
}

bool AddressBook::decodeJson( Json::Value& reader) {
   return g_proto->decodeJson(3, reader, onAddressBookDecodeJson, this);
}
