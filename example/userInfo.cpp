#include "userInfo.h"
#include "jmsg.h"
static const unsigned char s_protoString []= {
   0x50,0x65,0x72,0x73,0x6f,0x6e,0x20,0x3d,0x20,0x31,
   0x20,0x7b,0x0a,0x20,0x20,0x20,0x20,0x6e,0x61,0x6d,
   0x65,0x20,0x3a,0x20,0x73,0x74,0x72,0x69,0x6e,0x67,
   0x20,0x3d,0x20,0x31,0x0a,0x20,0x20,0x20,0x20,0x69,
   0x64,0x20,0x3a,0x20,0x69,0x6e,0x74,0x20,0x3d,0x20,
   0x32,0x0a,0x20,0x20,0x20,0x20,0x65,0x6d,0x61,0x69,
   0x6c,0x20,0x3a,0x20,0x73,0x74,0x72,0x69,0x6e,0x67,
   0x20,0x3d,0x20,0x33,0x0a,0x20,0x20,0x20,0x20,0x70,
   0x68,0x6f,0x6e,0x65,0x20,0x3a,0x20,0x5b,0x5d,0x50,
   0x68,0x6f,0x6e,0x65,0x4e,0x75,0x6d,0x62,0x65,0x72,
   0x20,0x3d,0x20,0x34,0x0a,0x20,0x20,0x20,0x20,0x61,
   0x64,0x64,0x72,0x65,0x73,0x73,0x20,0x3a,0x20,0x5b,
   0x5d,0x73,0x74,0x72,0x69,0x6e,0x67,0x20,0x3d,0x20,
   0x35,0x3b,0x0a,0x7d,0x0a,0x0a,0x50,0x68,0x6f,0x6e,
   0x65,0x4e,0x75,0x6d,0x62,0x65,0x72,0x20,0x3d,0x20,
   0x32,0x20,0x7b,0x0a,0x20,0x20,0x20,0x20,0x6e,0x75,
   0x6d,0x62,0x65,0x72,0x20,0x3a,0x20,0x73,0x74,0x72,
   0x69,0x6e,0x67,0x20,0x3d,0x20,0x31,0x0a,0x20,0x20,
   0x20,0x20,0x74,0x79,0x70,0x65,0x20,0x3a,0x20,0x69,
   0x6e,0x74,0x20,0x3d,0x20,0x32,0x0a,0x7d,0x0a,0x0a,
   0x41,0x64,0x64,0x72,0x65,0x73,0x73,0x42,0x6f,0x6f,
   0x6b,0x20,0x3d,0x20,0x33,0x20,0x7b,0x0a,0x20,0x20,
   0x20,0x20,0x70,0x65,0x72,0x73,0x6f,0x6e,0x20,0x3a,
   0x20,0x5b,0x5d,0x50,0x65,0x72,0x73,0x6f,0x6e,0x20,
   0x3d,0x20,0x31,0x0a,0x7d,0x00
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


static bool onPhoneNumberDecodeJson(JMsgProto* proto, JMsgField* field, rapidjson::Value& jsonValue, void* args) {
   bool isSuccess = false;
   PhoneNumber* value = (PhoneNumber*)args;
   switch(field->m_id) {
   case 1: {
      value->number = jsonValue.IsString() ? jsonValue.GetString() : "";
      break;
   }
   case 2: {
      value->type = jsonValue.IsInt() ? jsonValue.GetInt() : 0;
      break;
   }
   default:
      break;
   }
   return isSuccess;
}

static bool onPhoneNumberEncodeJson(JMsgProto* proto, JMsgField* field, rapidjson::Document& doc, rapidjson::Value& jsonValue, void* args) {
   auto& allocator = doc.GetAllocator();
   PhoneNumber* value = (PhoneNumber*)args;
   switch(field->m_id) {
   case 1: {
      rapidjson::Value valStr;
      valStr.SetString(value->number.c_str(), value->number.size(), allocator);
      jsonValue.AddMember("number", valStr.Move(), allocator);
      break;
   }
   case 2: {
      jsonValue.AddMember("type", value->type, allocator);
      break;
   }
   default:
      break;
   }
   return true;
}

void PhoneNumber::encodeJson( rapidjson::Document& doc, rapidjson::Value& writer) {
   g_proto->encodeJson(2, doc, writer, onPhoneNumberEncodeJson, this);
}

bool PhoneNumber::decodeJson(rapidjson::Value& reader) {
   return g_proto->decodeJson(2, reader, onPhoneNumberDecodeJson, this);
}
Person::Person() {
   m_msgId = 1;
   id = 0;
}


static bool onPersonDecodeJson(JMsgProto* proto, JMsgField* field, rapidjson::Value& jsonValue, void* args) {
   bool isSuccess = false;
   Person* value = (Person*)args;
   switch(field->m_id) {
   case 1: {
      value->name = jsonValue.IsString() ? jsonValue.GetString() : "";
      break;
   }
   case 2: {
      value->id = jsonValue.IsInt() ? jsonValue.GetInt() : 0;
      break;
   }
   case 3: {
      value->email = jsonValue.IsString() ? jsonValue.GetString() : "";
      break;
   }
   case 4: {
      for(size_t i = 0; i < jsonValue.Size(); i++) {
         PhoneNumber item;
         isSuccess = item.decodeJson(jsonValue[(int)i]);
         if(!isSuccess) break;
         value->phone.push_back(item);
      }
      break;
   }
   case 5: {
      for(size_t i = 0; i < jsonValue.Size(); i++) {
         value->address.push_back(jsonValue[(int)i].IsString() ? jsonValue[(int)i].GetString() : "");
      }
      break;
   }
   default:
      break;
   }
   return isSuccess;
}

static bool onPersonEncodeJson(JMsgProto* proto, JMsgField* field, rapidjson::Document& doc, rapidjson::Value& jsonValue, void* args) {
   auto& allocator = doc.GetAllocator();
   Person* value = (Person*)args;
   switch(field->m_id) {
   case 1: {
      rapidjson::Value valStr;
      valStr.SetString(value->name.c_str(), value->name.size(), allocator);
      jsonValue.AddMember("name", valStr.Move(), allocator);
      break;
   }
   case 2: {
      jsonValue.AddMember("id", value->id, allocator);
      break;
   }
   case 3: {
      rapidjson::Value valStr;
      valStr.SetString(value->email.c_str(), value->email.size(), allocator);
      jsonValue.AddMember("email", valStr.Move(), allocator);
      break;
   }
   case 4: {
      rapidjson::Value arrayValue(rapidjson::kArrayType);
      for(size_t i = 0; i < value->phone.size(); i++) {
         rapidjson::Value itemValue(rapidjson::kObjectType);
         value->phone[i].encodeJson(doc, itemValue);
         arrayValue.PushBack(itemValue.Move(), allocator);
      }
      jsonValue.AddMember("phone", arrayValue, allocator);
      break;
   }
   case 5: {
      rapidjson::Value arrayValue(rapidjson::kArrayType);
      for(size_t i = 0; i < value->address.size(); i++) {
         rapidjson::Value valStr;
         valStr.SetString(value->address[i].c_str(), value->address[i].size(), allocator);
         arrayValue.PushBack(valStr.Move(), allocator);
      }
      jsonValue.AddMember("address", arrayValue, allocator);
      break;
   }
   default:
      break;
   }
   return true;
}

void Person::encodeJson( rapidjson::Document& doc, rapidjson::Value& writer) {
   g_proto->encodeJson(1, doc, writer, onPersonEncodeJson, this);
}

bool Person::decodeJson(rapidjson::Value& reader) {
   return g_proto->decodeJson(1, reader, onPersonDecodeJson, this);
}
AddressBook::AddressBook() {
   m_msgId = 3;
}


static bool onAddressBookDecodeJson(JMsgProto* proto, JMsgField* field, rapidjson::Value& jsonValue, void* args) {
   bool isSuccess = false;
   AddressBook* value = (AddressBook*)args;
   switch(field->m_id) {
   case 1: {
      for(size_t i = 0; i < jsonValue.Size(); i++) {
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

static bool onAddressBookEncodeJson(JMsgProto* proto, JMsgField* field, rapidjson::Document& doc, rapidjson::Value& jsonValue, void* args) {
   auto& allocator = doc.GetAllocator();
   AddressBook* value = (AddressBook*)args;
   switch(field->m_id) {
   case 1: {
      rapidjson::Value arrayValue(rapidjson::kArrayType);
      for(size_t i = 0; i < value->person.size(); i++) {
         rapidjson::Value itemValue(rapidjson::kObjectType);
         value->person[i].encodeJson(doc, itemValue);
         arrayValue.PushBack(itemValue.Move(), allocator);
      }
      jsonValue.AddMember("person", arrayValue, allocator);
      break;
   }
   default:
      break;
   }
   return true;
}

void AddressBook::encodeJson( rapidjson::Document& doc, rapidjson::Value& writer) {
   g_proto->encodeJson(3, doc, writer, onAddressBookEncodeJson, this);
}

bool AddressBook::decodeJson(rapidjson::Value& reader) {
   return g_proto->decodeJson(3, reader, onAddressBookDecodeJson, this);
}
