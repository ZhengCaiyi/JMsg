#include "jmsg.h"
#include "userInfo.h"

using namespace std;
int main() {
	userInfoInit();
    JMsgWriter writer;
    AddressBook ab2;

	Person person1;
	person1.name = "Alice";
	person1.id = 10000;

	PhoneNumber phoneNo1;
	phoneNo1.number = "123456789";
	phoneNo1.type = 1;
	PhoneNumber phoneNo2;
	phoneNo2.number = "87654321";
	phoneNo2.type = 2;
    person1.phone = { phoneNo1, phoneNo2 };
    person1.address.push_back("xiamen");
    person1.address.push_back("tongan");
	ab2.person.push_back(person1);

	Person person2;
	person2.name = "Bob";
	person2.id = 20000;
	PhoneNumber phoneNo3;
	phoneNo3.number = "01234567890";
	phoneNo3.type = 3;
	person2.phone.push_back(phoneNo3);
	ab2.person.push_back(person2);
	
    rapidjson::Document doc;
    rapidjson::Value jsonValue(rapidjson::kObjectType);
	ab2.encodeJson(doc, jsonValue);


    rapidjson::StringBuffer buffer;
    buffer.Clear();
    rapidjson::Writer<rapidjson::StringBuffer> bufferwriter(buffer);
    jsonValue.Accept(bufferwriter);
    auto data = buffer.GetString();

    AddressBook ab3;
    JMsgFromJson(data, ab3);
    userInfoFini();
}