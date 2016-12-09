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
   void encode(JMsgProto* proto, JMsgWriter* writer);
   bool decode(JMsgProto* proto, JMsgReader* reader);
};
