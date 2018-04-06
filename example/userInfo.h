#ifndef userInfo_h
#define userInfo_h
#include <stdio.h>
#include <vector>
#include <string>
#include "json/value.h"
#include "jmsg_encodeable.h"
using namespace std;
class JMsgWriter;
class JMsgReader;
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
   string number;
   int type;
   virtual void encode(JMsgWriter* writer);
   virtual bool decode(JMsgReader* reader);
   virtual void encodeJson( Json::Value& val);
   virtual bool decodeJson( Json::Value& val);
};

class Person : public IJMsgEncodeable{
public:
   Person();
   string name;
   int id;
   string email;
   std::vector<PhoneNumber> phone;
   virtual void encode(JMsgWriter* writer);
   virtual bool decode(JMsgReader* reader);
   virtual void encodeJson( Json::Value& val);
   virtual bool decodeJson( Json::Value& val);
};

class AddressBook : public IJMsgEncodeable{
public:
   AddressBook();
   std::vector<Person> person;
   virtual void encode(JMsgWriter* writer);
   virtual bool decode(JMsgReader* reader);
   virtual void encodeJson( Json::Value& val);
   virtual bool decodeJson( Json::Value& val);
};

#endif
