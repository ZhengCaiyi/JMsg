#include <stdio.h>
#include <vector>
#include "jmsg_idl_parse.h"
#include "jmsg_util.h"
#include "jmsg_proto.h"
#include "jmsg_writer.h"
#include "jmsg_reader.h"
#include "jmsg_field.h"

using namespace std;
struct AddressInfo {
	string street;
	int number;
};

struct UserInfo {
	string userName;
	string password;
	vector<AddressInfo> addresses;
	int age;
};

bool addressEncodeCallback(JMsgProto* proto, JMsgField* field, JMsgWriter* writer, void* args) {
	AddressInfo* addressInfo = (AddressInfo*)args;
	switch(field->m_id) {
	case 1:
		printf("addressEncodeCallback, setting street=%s\n", addressInfo->street.c_str());
		writer->writeStringField(field, addressInfo->street);
		break;
	case 2:
	printf("addressEncodeCallback, setting street=%d\n", addressInfo->number);
		writer->writeIntField(field, addressInfo->number);
		break;
	}
	return true;
}

bool addressDecodeCallback(JMsgProto* proto, JMsgField* field, JMsgReader* reader, void* args) {
	AddressInfo* addressInfo = (AddressInfo*)args;
	printf("addressDecodeCallback,fieldId=%d, typeId=%d\n", field->m_id, field->m_typeId);
	switch(field->m_id) {
	case 1:
		addressInfo->street = reader->readString();
		printf("read street = %s\n", addressInfo->street.c_str());
		break;
	case 2:
		addressInfo->number = reader->readInt();
		printf("read number = %d\n", addressInfo->number);
		break;
	}
	return true;
}

bool userInfoEncodeCallback(JMsgProto* proto, JMsgField* field, JMsgWriter* writer, void* args) {
	UserInfo* userInfo = (UserInfo*)args;

	switch(field->m_id) {
		case 1:
				writer->writeStringField(field, userInfo->userName);
				return true;
			case 2:
				writer->writeStringField(field, userInfo->password);
				return true;
			case 3:
				writer->writeArrayHeader(field, userInfo->addresses.size());
			     for(int i = 0; i < userInfo->addresses.size(); i++) {
			     	proto->encode(field->m_typeId, writer, addressEncodeCallback,  &userInfo->addresses[i]);
			     }
			     return true;
		     case 4:
		     	writer->writeIntField(field, userInfo->age);
		     	return true;
			default:
				return false;
	}
	return true;
}

bool userInfoDecodeCallback(JMsgProto* proto, JMsgField* field, JMsgReader* reader, void* args) {
	UserInfo* userInfo = (UserInfo*)args;
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
			     printf("read array length:%d\n", arrayLen);
			     for(int i = 0; i < arrayLen; i++) {
			     	AddressInfo addrInfo;
			     	proto->decode(reader, addressDecodeCallback, &addrInfo);
			     	userInfo->addresses.push_back(addrInfo);
			     }
			     return true;
			 }
			 case 4: {
			 	userInfo->age = reader->readInt();
			 	return true;
			 }
			default:
				return false;

	}
	return true;
}


int main() {
	UserInfo userInfoEncode;
	UserInfo userInfoDecode;



	userInfoEncode.userName = "Jacky";
	userInfoEncode.password = "12s3";
	userInfoEncode.age = 129;
	
	for(int i = 0; i < 200; i++) {
		AddressInfo address;
		address.street = ""; 
		address.number = i + 1;
		userInfoEncode.addresses.push_back(address);
	}

	string* content = jMsgGetFileString("config1.txt");
	JMsgProto* proto = JMsgProto::createProto(*content);

	if(!proto) {
		printf("parse config file failed\n");
	}

	//for(int i = 0; i < 1000000; i++) {
	JMsgWriter writer;
	proto->encode("UserInfo", &writer, userInfoEncodeCallback, &userInfoEncode);

	JMsgReader reader((unsigned char*)writer.getBuffer(), writer.getBufferLen());
    proto->decode(&reader, userInfoDecodeCallback, &userInfoDecode);
	//}
	
	printf("decoded userName=%s, password=%s, age=%d\n", userInfoDecode.userName.c_str(), userInfoDecode.password.c_str(), userInfoDecode.age);
	for(int i = 0; i < userInfoDecode.addresses.size(); i++) {
	 	printf("  address %d: street=%s, number=%d\n", i, userInfoDecode.addresses[i].street.c_str(),  userInfoDecode.addresses[i].number);
	}
	delete proto;
	delete content;
	//printf("encoded len = %d\n", writer.getBufferLen());

	//JMsgReader reader((unsigned char*)writer.getBuffer(), writer.getBufferLen());
	//proto->decode(&reader, decodeCallback, &userInfoDecode);
	
	//printf("userName=%s, password=%s, age.count=%d\n", userInfoDecode.userName.c_str(), userInfoDecode.password.c_str(), userInfoDecode.age.size());
}