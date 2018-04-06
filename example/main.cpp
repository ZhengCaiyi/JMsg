#include "jmsg.h"
#include "userInfo.h"
#include <json/value.h>
#include <json/writer.h>
using namespace std;
int main() {
	userInfoInit();
    JMsgWriter writer;
    AddressBook ab;

	Person person1;
	person1.name = "Alice";
	person1.id = 10000;

	PhoneNumber phoneNo1;
	phoneNo1.number = "123456789";
	phoneNo1.type = 1;
	PhoneNumber phoneNo2;
	phoneNo2.number = "87654321";
	phoneNo2.type = 2;
	person1.phone.push_back(phoneNo1);
	person1.phone.push_back(phoneNo2);
	ab.person.push_back(person1);

	Person person2;
	person2.name = "Bob";
	person2.id = 20000;
	PhoneNumber phoneNo3;
	phoneNo3.number = "01234567890";
	phoneNo3.type = 3;
	person2.phone.push_back(phoneNo3);
	ab.person.push_back(person2);
	
	ab.encode(&writer);	

	JMsgReader reader((unsigned char*)writer.getBuffer(), writer.getBufferLen());
	AddressBook ab2;
	ab2.decode(&reader);

	Json::Value jsonValue;
	Json::FastWriter jsonWriter;
	ab2.encodeJson(jsonValue);
	string jsonString = jsonWriter.write(jsonValue);
	printf("userInfo:\n%s", jsonString.c_str());
	userInfoFini();
	getchar();
}