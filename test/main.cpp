#include <stdio.h>
#include <vector>
#include "jmsg_idl_parse.h"
#include "jmsg_util.h"
#include "jmsg_proto.h"
#include "jmsg_writer.h"
#include "jmsg_reader.h"
#include "jmsg_field.h"
#include "DBMSGs.h"
#include "json/writer.h"
/*
#include "config1.h"
#include "config2.h"
using namespace std;
int main() {

	JMsgProto* proto2 = config2CreateProto();
	proto2->setFixFieldLen(true);
	UserInfo2 userInfo2;
	userInfo2.userName = "caiyi";
	userInfo2.age = 10;
	userInfo2.password = "123";

	for (int i = 0; i < 1; i++) {
		AddressInfo2 addrInfo2;
		addrInfo2.number = 12;
		addrInfo2.street = "123";
		addrInfo2.newField = "helloWorld";
		userInfo2.addresses.push_back(addrInfo2);
	}
	JMsgWriter writer;
	userInfo2.encode(proto2, &writer);

	JMsgProto* proto1 = config1CreateProto();
	proto1->setFixFieldLen(true);
	JMsgReader reader((unsigned char*)writer.getBuffer(), writer.getBufferLen());
	string strJson;
	proto1->toJson(&reader, 0, strJson);
}
*/

int main() {
	JMsgProto* proto = DBMSGsCreateProto();
	DBMSGUpdatePlayerData upd;
	DBMSGPlayerInfo playerInfo;
	playerInfo.nickName = "nihao";
	playerInfo.readMailIds.push_back(1);
	playerInfo.readMailIds.push_back(2);
	playerInfo.allowanceRate = 3.3;
	for(int i = 0; i <10; i++) {
		DBMSGProp prop;
		prop.propId = i;
		prop.count = 1;
		playerInfo.props.push_back(prop);
	}
	upd.playerInfo = playerInfo;
	
	Json::Value val;

	//val["ad"] = 1;
	DBMSGProp propTest;
	propTest.count = 1;
	propTest.propId = 2;
	propTest.encodeJson(proto, val);
	Json::FastWriter fastWriter;

	Json::Value trans;
	trans["type"]= "DBMSGProp";
	trans["data"] = val;
	printf("encoded=%s\n", trans.toStyledString().c_str());

	DBMSGUpdatePlayerData upd2;
	upd2.decodeJson(proto, val);
	getchar();

}