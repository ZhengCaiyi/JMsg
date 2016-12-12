--local jmsg = require 'jmsg'
local idl_string = [[
UserInfo = 12 {
	userName: string = 1
	password: string = 2
	addresses: []AddressInfo = 3
	age: int = 4
}

AddressInfo = 10 {
	street: string =  1
	number: int = 2
}
]]
local proto = jmsg.create(idl_string)
local data = {
	userName = "hello",
	password = "123",
	age = 4,
	addresses = {
		{
			street = "street1",
			number = 1
		}, 
		{
			street = "street2",
			number = 2
		}
	}
}
local encoded = jmsg.encode(proto, "UserInfo", data)
for i = 1, 100000 do	
	local decoded = jmsg.decode(proto, encoded)
end
jmsg.print("end test")
--jmsg.print("decoded.userName="..decoded.userName)
--jmsg.print("decoded.password="..decoded.password)
---for i = 1, #decoded.addresses do
--	jmsg.print("  address.street="..decoded.addresses[i].street)
--end
return encoded