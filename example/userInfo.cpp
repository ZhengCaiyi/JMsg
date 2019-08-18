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
      if(jsonValue.IsString()) value->number = jsonValue.GetString();
      break;
   }
   case 2: {
      if(jsonValue.IsInt()) value->type = jsonValue.GetInt();
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
      if(!value->number.has_value()) break;
      rapidjson::Value valStr;
      valStr.SetString(value->number.value().c_str(), value->number.value().size(), allocator);
      jsonValue.AddMember("number", valStr.Move(), allocator);
      break;
   }
   case 2: {
      if(!value->type.has_value()) break;
      jsonValue.AddMember("type", value->type.value(), allocator);
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
      if(jsonValue.IsString()) value->name = jsonValue.GetString();
      break;
   }
   case 2: {
      if(jsonValue.IsInt()) value->id = jsonValue.GetInt();
      break;
   }
   case 3: {
      if(jsonValue.IsString()) value->email = jsonValue.GetString();
      break;
   }
   case 4: {
      std::vector<PhoneNumber> arrayValue;
      for(size_t i = 0; i < jsonValue.Size(); i++) {
         PhoneNumber item;
         isSuccess = item.decodeJson(jsonValue[(int)i]);
         if(!isSuccess) break;
         arrayValue.push_back(item);
         value->phone = arrayValue;
      }
      break;
   }
   case 5: {
      std::vector<string> arrayValue;
      for(size_t i = 0; i < jsonValue.Size(); i++) {
         arrayValue.push_back(jsonValue[(int)i].IsString() ? jsonValue[(int)i].GetString() : "");
         value->address = arrayValue;
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
      if(!value->name.has_value()) break;
      rapidjson::Value valStr;
      valStr.SetString(value->name.value().c_str(), value->name.value().size(), allocator);
      jsonValue.AddMember("name", valStr.Move(), allocator);
      break;
   }
   case 2: {
      if(!value->id.has_value()) break;
      jsonValue.AddMember("id", value->id.value(), allocator);
      break;
   }
   case 3: {
      if(!value->email.has_value()) break;
      rapidjson::Value valStr;
      valStr.SetString(value->email.value().c_str(), value->email.value().size(), allocator);
      jsonValue.AddMember("email", valStr.Move(), allocator);
      break;
   }
   case 4: {
      if(!value->phone.has_value()) break;
      rapidjson::Value arrayValue(rapidjson::kArrayType);
      for(size_t i = 0; i < value->phone.value().size(); i++) {
         rapidjson::Value itemValue(rapidjson::kObjectType);
         value->phone.value()[i].encodeJson(doc, itemValue);
         arrayValue.PushBack(itemValue.Move(), allocator);
      }
      jsonValue.AddMember("phone", arrayValue, allocator);
      break;
   }
   case 5: {
      if(!value->address.has_value()) break;
      rapidjson::Value arrayValue(rapidjson::kArrayType);
      for(size_t i = 0; i < value->address.value().size(); i++) {
         rapidjson::Value valStr;
         valStr.SetString(value->address.value()[i].c_str(), value->address.value()[i].size(), allocator);
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
      std::vector<Person> arrayValue;
      for(size_t i = 0; i < jsonValue.Size(); i++) {
         Person item;
         isSuccess = item.decodeJson(jsonValue[(int)i]);
         if(!isSuccess) break;
         arrayValue.push_back(item);
         value->person = arrayValue;
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
      if(!value->person.has_value()) break;
      rapidjson::Value arrayValue(rapidjson::kArrayType);
      for(size_t i = 0; i < value->person.value().size(); i++) {
         rapidjson::Value itemValue(rapidjson::kObjectType);
         value->person.value()[i].encodeJson(doc, itemValue);
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
