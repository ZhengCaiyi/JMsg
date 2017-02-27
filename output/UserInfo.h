#ifndef UserInfo_h
#define UserInfo_h
#include <stdio.h>
#include <vector>
#include <string>
#include "AddressInfo.h"
using namespace std;
class JMsgWriter;
class JMsgReader;
class JMsgProto;
struct UserInfo {
   string userName;
   string password;
   std::vector<AddressInfo> addresses;
   int age;
   bool sex;
   void encode(JMsgProto* proto, JMsgWriter* writer);
   bool decode(JMsgProto* proto, JMsgReader* reader);
};
#endif
