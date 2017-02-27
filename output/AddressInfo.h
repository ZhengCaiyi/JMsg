#ifndef AddressInfo_h
#define AddressInfo_h
#include <stdio.h>
#include <vector>
#include <string>
using namespace std;
class JMsgWriter;
class JMsgReader;
class JMsgProto;
struct AddressInfo {
   string street;
   int number;
   void encode(JMsgProto* proto, JMsgWriter* writer);
   bool decode(JMsgProto* proto, JMsgReader* reader);
};
#endif
