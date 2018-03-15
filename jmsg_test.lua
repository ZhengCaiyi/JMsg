--local jmsg = require 'jmsg'
local idl_string1 = [[
UserInfo = 12 {
	userName: string = 1
	password: string = 2
	addresses: AddressInfo = 3
	age: int = 4
	sex: bool = 5
}

AddressInfo = 10 {
	street: string = 1
	number: int = 2
	newField : NewField = 3
}

NewField = 11 {
	name : string = 1
	value : int = 2
}
]]

local idl_string2 = [[
UserInfo = 12 {
	userName: string = 1
	password: string = 2
	addresses: AddressInfo = 3
	age: int = 4
	sex: bool = 5
}

AddressInfo = 10 {
	street: string =  1
	number: int = 2
}
]]

local proto1 = jmsg.create(idl_string1)
local proto2 = jmsg.create(idl_string2)

local data = {
	userName = "hello",
	password = "123",
	age = 4,
	addresses =	{
			street = "street1",
			number = 1,
			newField = {
				name = "newFieldName",
				value = 3
			}
	},
	sex = true
}
local encoded = jmsg.encode(proto1, "UserInfo", data)
local decoded = jmsg.decode(proto2, encoded)
jmsg.print("end test")
jmsg.print("decoded.userName="..decoded.userName)
jmsg.print("decoded.password="..decoded.password)
jmsg.print("decoded.age"..decoded.age)
jmsg.print("address")
jmsg.close(proto1)
jmsg.close(proto2)
return encoded