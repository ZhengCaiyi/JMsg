#ifndef userInfo_h
#define userInfo_h
#include <stdio.h>
#include <vector>
#include <string>
#include <absl/types/optional.h>
#include "jmsg_encodeable.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
using namespace std;
class JMsgProto;

enum userInfoTypeIds {
   kPhoneNumber = 2,
   kPerson = 1,
   kAddressBook = 3,
};

JMsgProto* userInfoCreateProto();
void userInfoInit();
void userInfoFini();
JMsgProto* userInfoGetProto();
class PhoneNumber : public IJMsgEncodeable{
public:
   PhoneNumber();
   absl::optional<string> number;
   absl::optional<int> type;
   virtual void encodeJson(rapidjson::Document& doc, rapidjson::Value& val);
   virtual bool decodeJson(rapidjson::Value& val);
};

class Person : public IJMsgEncodeable{
public:
   Person();
   absl::optional<string> name;
   absl::optional<int> id;
   absl::optional<string> email;
   absl::optional<std::vector<PhoneNumber>> phone;
   absl::optional<std::vector<string>> address;
   virtual void encodeJson(rapidjson::Document& doc, rapidjson::Value& val);
   virtual bool decodeJson(rapidjson::Value& val);
};

class AddressBook : public IJMsgEncodeable{
public:
   AddressBook();
   absl::optional<std::vector<Person>> person;
   virtual void encodeJson(rapidjson::Document& doc, rapidjson::Value& val);
   virtual bool decodeJson(rapidjson::Value& val);
};

#endif
