extern "C" {
	#include "lua.h"
	#include "lauxlib.h"
}

#include "jmsg.h"
using namespace std;
bool encodeCallback(JMsgProto* proto, JMsgField* field, JMsgWriter* writer, void* args) {
	lua_State* L = (lua_State*)args;
	lua_getfield(L, -1, field->m_name.c_str());
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		return false;
	}

	if(field->m_isArray) {
		int len = luaL_len(L, -1); //luaL_getn(L, -1);
		writer->writeArrayHeader(field, len);
		
		for(int i = 1; i <= len; i++) {
			::lua_pushinteger(L, i);
			::lua_gettable(L, -2);
			if(field->m_type == "string") {
				const char* value = luaL_checkstring(L, -1);

				if(value) {
					writer->writeString(value);
				}
			} else if(field->m_type == "int") {
				int value = ::luaL_checkinteger(L, -1);
				writer->writeInt(value);
			} else if(field->m_type == "double") {
				double value = ::luaL_checknumber(L, -1);
				writer->writeDouble(value);
			} else if(field->m_type == "bool") {
				bool value = ::lua_toboolean(L, -1);
				writer->writeBool(value);
			} else {
				proto->encode(field->m_typeId, writer, encodeCallback, L);
			}
			lua_pop(L, 1);
		}
	} else if(field->m_type == "string") {
		const char* value = luaL_checkstring(L, -1);

		if(value) {
			writer->writeStringField(field, value);
		}
	} else if(field->m_type == "int") {
		int value = ::luaL_checkinteger(L, -1);
		writer->writeIntField(field, value);
	} else if(field->m_type == "double") {
		double value = ::luaL_checknumber(L, -1);
		writer->writeDoubleField(field, value);
	} else if(field->m_type == "bool") {
		bool value = ::lua_toboolean(L, -1);
		writer->writeBoolField(field, value);
	} else {
		writer->writeFieldHeader(field);
		proto->encode(field->m_typeId, writer, encodeCallback, L);
	}
	lua_pop(L, 1);
	return true;
}

bool decodeCallback(JMsgProto* proto, JMsgField* field, JMsgReader* reader, void* args) {
	lua_State* L = (lua_State*)args;
	bool isSuccess = false;
	::lua_pushstring(L, field->m_name.c_str());
	if(field->m_isArray) {
		//printf("m_isArray, top=%d", lua_gettop(L));
		::lua_newtable(L);

		int len = reader->readArrayLength(isSuccess);

		for(int i = 1; i <= len; i++) {
			::lua_pushinteger(L, i);
			if(field->m_type == "string") {
				string value = reader->readString(isSuccess);
				::lua_pushstring(L, value.c_str());
			} else if(field->m_type == "int") {
				int value = reader->readInt(isSuccess);
				::lua_pushinteger(L, value);
			} else if(field->m_type == "bool") {
				bool value = reader->readBool(isSuccess);
				::lua_pushboolean(L, value);
			} else if(field->m_type == "double") {
				double value = reader->readDouble(isSuccess);
				::lua_pushnumber(L, value);
			} else {
				::lua_createtable(L, 0, 0);
				proto->decode(reader, decodeCallback, L);
				//printf("after decode, top=%d", lua_gettop(L));
			}

			::lua_settable(L, -3);
		}

	} else if(field->m_type == "string") {
		string value = reader->readString(isSuccess);
		::lua_pushstring(L, value.c_str());
	} else if(field->m_type == "int") {
		int value = reader->readInt(isSuccess);
		lua_pushinteger(L, value);
	} else if(field->m_type == "bool") {
		bool value = reader->readBool(isSuccess);
		lua_pushboolean(L, value);
	} else if(field->m_type == "double") {
		double value = reader->readDouble(isSuccess);
		lua_pushnumber(L, value);
	} else {
		::lua_createtable(L, 0, 0);
		proto->decode(reader, decodeCallback, L);
	}
	lua_settable(L, -3);
	//printf("callback end, top=%d\n", lua_gettop(L));
	return true;
}



extern "C" static int lcreate(lua_State* L) {
	const char* str = luaL_checkstring(L, -1);
	JMsgProto* proto = JMsgProto::createProto(str);

	if(!proto) {
		return 0;
	}
	lua_pop(L, 1);
	::lua_pushlightuserdata(L, proto);
	return 1;
}

extern "C" static int lprint(lua_State* L) {
	const char* str = luaL_checkstring(L, -1);
	printf("%s\n", str);
	return 0;
}

extern "C" static int lencodeBinary(lua_State* L) {
	JMsgWriter writer;
	JMsgProto* proto = (JMsgProto*)::lua_touserdata(L, -3);
	const char* typeName = luaL_checkstring(L, -2);
	bool success = proto->encode(typeName, &writer, &encodeCallback, L);
	lua_pop(L, 3);
	if (success) {
		char* data = new char[writer.getBufferLen()];
		memcpy(data, writer.getBuffer(), writer.getBufferLen());

		lua_pushlightuserdata(L, data);
		lua_pushinteger(L, writer.getBufferLen());
		return 2;
	}
	else {
		return 0;
	}

}

extern "C" static int lfreeBinary(lua_State* L) {
	char* userData = (char*)lua_touserdata(L, -1);
	delete userData;
	lua_pop(L, 1);
	return 0;
}

extern "C" static int lencode(lua_State* L) {
	JMsgWriter writer;
	JMsgProto* proto = (JMsgProto*)::lua_touserdata(L, -3);
	const char* typeName = luaL_checkstring(L, -2);
	bool success = proto->encode(typeName, &writer, &encodeCallback, L);
	lua_pop(L, 3);
	if(success) {
		::lua_pushlstring(L, writer.getBuffer(), writer.getBufferLen());
		return 1;
	} else {
		return 0;
	}
	
	return 1;
}

extern "C" static int ldecode(lua_State* L) {
	JMsgProto* proto = (JMsgProto*)::lua_touserdata(L, -2);
	size_t len = 0;
	const char* ptr = luaL_checklstring(L, -1, &len);
	if(!proto) {
		return 0;
	}
	lua_pop(L, 2);
	::lua_createtable(L, 0, 0);
	::JMsgReader reader((unsigned char*)ptr, len);	
	int typeId = proto->decode(&reader, decodeCallback, L);

	JMsgType* msgType = proto->getTypeById(typeId);

	lua_pushstring(L, msgType ? msgType->m_typeName.c_str() : "");
	return 2;
}

extern "C" static int ldecodeBinary(lua_State* L) {
	JMsgProto* proto = (JMsgProto*)::lua_touserdata(L, -2);
	size_t len = 0;
	const char* ptr = (char*)lua_touserdata(L, -1);
	if (!proto) {
		return 0;
	}
	lua_pop(L, 2);
	::lua_createtable(L, 0, 0);
	::JMsgReader reader((unsigned char*)ptr, -1);

	int typeId = proto->decode(&reader, decodeCallback, L);
	JMsgType* msgType = proto->getTypeById(typeId);
	lua_pushstring(L, msgType ? msgType->m_typeName.c_str() : "");
	return 2;
}

extern "C" static int lclose(lua_State* L) {
	JMsgProto* proto = (JMsgProto*)::lua_touserdata(L, -2);
	lua_pop(L, 1);
	delete proto;
	return 0;
}


static const struct luaL_Reg s_functions[] = {
	{ "create", lcreate },
	{ "encode", lencode },
	{ "encodeBinary", lencodeBinary },
	{ "freeBinary", lfreeBinary },
	{ "decode", ldecode },
	{ "decodeBinary", ldecodeBinary },
	{ "close", lclose },
	{ "print", lprint },
	{ NULL, NULL }
};

int JMsgRegisterLua(lua_State* L) {	
	//luaL_register(L, "jmsg", s_functions);
	//lua_register(L, "jmsg", s_functions);
	luaL_newlib(L, s_functions);
	return 1;
}


int main() {
	lua_State* L = luaL_newstate();
	luaL_requiref(L, "jmsg", JMsgRegisterLua, 1);
	int ret = luaL_dofile(L, "e:/JMsg/jmsg_test.lua");

	if(ret != 0) {
		printf("Ö´ÐÐÎÄ¼þÊ§°Ü:%s\n", luaL_checkstring(L, -1));
	}
	lua_close(L);
	getchar();
}