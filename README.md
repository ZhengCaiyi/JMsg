JMsg - A light weight message encode/decode library
================================
Overview
--------------------------------
JMsg is a message encode/decode library which is simalar to Google Protobuf.

#Features
* Define message format with JMsg proto file
* Support serialization of both binary and json format(with jsoncpp)
* Support C++(auto generate encode/decode code) and lua(C binding)

#Compile
The code struct is very easy. All the library code files are located in the src directory. Code generater refrence the library.
###### Code Generator:
* Linux:just type make in the generator directory
* Windows:we have provide Visual Studio 2008 project in the code. You can upgrade to any version you want.

###### Library:
Add all files  of src to your project.
##Usage
First of all, write the proto file, as a example, userInfo.txt below
````c++
UserInfo = 1 {
    userName : string = 1
    address : AddressInfo = 2
    items : []ItemInfo = 3
    age : int = 4
}

AddressInfo = 2 {
    street : string = 1
    country : string = 2
    phoneNo : string = 3
}

ItemInfo = 2 {
    itemId : int = 1
    itemCount : int = 2
}
````

then generate the c++ file with command
````
JMsg userInfo.txt userInfo ./ both
````
JMsg will generate userInfo.h and userInfo.cpp in the directory that we set in the command line.
````C++
#include "jmsg.h"
#include "userInfo.h"
#include <json/value.h>
#include <json/writer.h>

using namespace std;
int main() {
    userInfoInit();
    JMsgWriter writer;
    UserInfo oldUserInfo;
    oldUserInfo.userName = "jack";
    oldUserInfo.address.street = "NaJing";
    oldUserInfo.address.country = "China";
    oldUserInfo.address.phoneNo = "1234565";
    oldUserInfo.age = 10; 
    
    ItemInfo item;
    item.itemId = 1;
    item.itemCount = 2;
    oldUserInfo.items.push_back(item);

    // encode to binary data
    oldUserInfo.encode(&writer);

    JMsgReader reader((unsigned char*)writer.getBuffer(), writer.getBufferLen());
    UserInfo newUserInfo;
    // decode from binary data
    newUserInfo.decode(&reader);

    Json::Value jsonValue;
    // encode to json
    newUserInfo.encodeJson(jsonValue);
    string jsonString = jsonValue.toStyledString();
    printf("userInfo:\n%s", jsonString.c_str());
    userInfoFini();
    getchar();
}
````
output:
````json
userData:
{
   "address" : {
      "country" : "China",
      "phoneNo" : "1234565",
      "street" : "NaJing"
   },
   "age" : 10,
   "items" : [
      {
         "itemCount" : 2,
         "itemId" : 1
      }
   ],
   "userName" : "jack"
}
````
