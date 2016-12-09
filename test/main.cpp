#include <stdio.h>
#include <vector>
#include "jmsg_idl_parse.h"
#include "jmsg_util.h"
#include "jmsg_proto.h"
#include "jmsg_writer.h"
#include "jmsg_reader.h"
#include "jmsg_field.h"
 
using namespace std;

struct UserInfo {
	string userName;
	string password;
	vector<int> age;
};

bool encodeCallback(JMsgField* field, JMsgWriter* writer, void* args) {
	UserInfo* userInfo = (UserInfo*)args;
	//printf("encodeCallback, name=%s\n", field->m_name.c_str());
	switch(field->m_id) {
			case 1:
				writer->writeFieldId(field->m_id);
				writer->writeString(userInfo->userName);
				return true;
			case 2:
			    writer->writeFieldId(field->m_id);
				writer->writeString(userInfo->password);
				return true;
			case 3:
			     writer->writeFieldId(field->m_id);
			     writer->writeFieldId(userInfo->age.size());
			     for(int i = 0; i < userInfo->age.size(); i++) {
			     	writer->writeInt(userInfo->age[i]);
			     }
			     return true;
			default:
				return false;
	}
}

bool decodeCallback(JMsgField* field, JMsgReader* reader, void* args) {
	UserInfo* userInfo = (UserInfo*)args;
	//printf("decodeCallback, name=%s\n", field->m_name.c_str());
	switch(field->m_id) {
			case 1:
			{
				userInfo->userName = reader->readString();
				return true;
			}
			case 2:
			{
			    userInfo->password = reader->readString();
				return true;
			}
			case 3:
			{
			     int arrayLen = reader->readArrayLength();
			     for(int i = 0; i < arrayLen; i++) {
			     	userInfo->age.push_back(reader->readInt());
			     }
			     return true;
			 }
			default:
				return false;

	}

}


int main() {
	UserInfo userInfoEncode;
	UserInfo userInfoDecode;

	userInfoEncode.userName = "Jacky";
	userInfoEncode.password = "123";
	userInfoEncode.age.push_back(1);
	userInfoEncode.age.push_back(2);
	vector<JMsgType*> vecTypes;
	string* content = jMsgGetFileString("config1.txt");
	JMsgProto* proto = JMsgProto::createProto(*content);

	if(!proto) {
		printf("parse config file failed\n");
	}

	for(int i = 0; i < 10000000; i++) {
		JMsgWriter writer;
		proto->encode("UserInfo", &writer, encodeCallback, &userInfoEncode);

		JMsgReader reader((unsigned char*)writer.getBuffer(), writer.getBufferLen());
	    proto->decode(&reader, decodeCallback, &userInfoDecode);
	}
	printf("end\n");
	//printf("encoded len = %d\n", writer.getBufferLen());

	//JMsgReader reader((unsigned char*)writer.getBuffer(), writer.getBufferLen());
	//proto->decode(&reader, decodeCallback, &userInfoDecode);
	
	//printf("userName=%s, password=%s, age.count=%d\n", userInfoDecode.userName.c_str(), userInfoDecode.password.c_str(), userInfoDecode.age.size());
}