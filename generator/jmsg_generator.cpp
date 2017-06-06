#include "jmsg.h"
#include "jmsg_util.h"
#include "jmsg_code_writer.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
}
#include <string>
#include <set>
using namespace std;

void writeClassDeclare(const string& baseDir, JMsgType* type, JMSGCodeWriter& writer) {	
	writer.writeLine("class %s : public IJMsgEncodeable{", type->m_typeName.c_str());
	writer.writeLine("public:");
	writer.addIndent();
	writer.writeLine("%s();", type->m_typeName.c_str());
	for(size_t i = 0; i < type->m_vecFields.size(); i++) {
		JMsgField* field = type->m_vecFields[i];
		if(!field->m_isArray) {
			writer.writeLine("%s %s;", field->m_type.c_str(), field->m_name.c_str());
		} else {
			writer.writeLine("std::vector<%s> %s;", field->m_type.c_str(), field->m_name.c_str());
		}
	}
	
	writer.writeLine("virtual void encode(JMsgProto* proto, JMsgWriter* writer);");
	writer.writeLine("virtual bool decode(JMsgProto* proto, JMsgReader* reader);");
	
	writer.removeIndent();
	writer.writeLine("};");
	writer.writeLine("");
}

void writeClassImplement(const string& baseDir, JMsgType* type, JMSGCodeWriter& writer) {

	writer.writeLine("%s::%s() {", type->m_typeName.c_str(), type->m_typeName.c_str());
	writer.addIndent();
	writer.writeLine("m_msgId = %d;", type->m_id);

	for(size_t i = 0; i < type->m_vecFields.size(); i++) {
		JMsgField* field = type->m_vecFields[i];
		if(field->m_isArray) {
			continue;
		}

		if(field->m_type == "double" || field->m_type == "int") {
			writer.writeLine("%s = 0;", field->m_name.c_str());
		} else if(field->m_type == "bool") {
			writer.writeLine("%s = false;", field->m_name.c_str());
		}
	}
	writer.removeIndent();
	writer.writeLine("}");
	writer.writeLine("");

	
		
	

	// start decode
	writer.writeLine("static bool on%sDecode(JMsgProto* proto, JMsgField* field, JMsgReader* reader, void* args) {", type->m_typeName.c_str());
	writer.addIndent();
	if(type->m_vecFields.size() != 0) {
		writer.writeLine("bool isSuccess = false;");
		writer.writeLine("%s* value = (%s*)args;", type->m_typeName.c_str(),  type->m_typeName.c_str());
		writer.writeLine("switch(field->m_id) {");
		for(size_t i = 0; i < type->m_vecFields.size(); i++) {
			JMsgField* field = type->m_vecFields[i];
			printf("generating field:%s\n", field->m_name.c_str());
			writer.writeLine("case %d: {", field->m_id);
			writer.addIndent();
			
			if(field->m_isArray) {
				writer.writeLine("int arrayLen = reader->readArrayLength(isSuccess);");
				writer.writeLine("if(!isSuccess) break;");
				writer.writeLine("for(int i = 0; i < arrayLen; i++) {");
				writer.addIndent();
				 if(field->m_type == "string") {
					writer.writeLine("value->%s.push_back(reader->readString(isSuccess));", field->m_name.c_str());
				} else if(field->m_type == "int") {
					writer.writeLine("value->%s.push_back(reader->readInt(isSuccess));", field->m_name.c_str());
				} else if(field->m_type == "bool") {
					writer.writeLine("value->%s.push_back(reader->readBool(isSuccess));", field->m_name.c_str());
				} else if(field->m_type == "double"){
					writer.writeLine("value->%s.push_back(reader->readDouble(isSuccess));", field->m_name.c_str());
				} else if(field->m_typeId != 0) {
					writer.writeLine("%s item;", field->m_type.c_str());
					writer.writeLine("isSuccess = item.decode(proto, reader);");
					writer.writeLine("if(!isSuccess) break;");
					writer.writeLine("value->%s.push_back(item);", field->m_name.c_str());
				} 
				writer.removeIndent();
				writer.writeLine("}");
			} else if(field->m_type == "string") {
				writer.writeLine("value->%s = reader->readString(isSuccess);", field->m_name.c_str());
			} else if(field->m_type == "bool") {
				writer.writeLine("value->%s = reader->readBool(isSuccess);", field->m_name.c_str());
			} else if(field->m_type == "int") {
				writer.writeLine("value->%s = reader->readInt(isSuccess);", field->m_name.c_str());
			} else if(field->m_type == "double") {
				writer.writeLine("value->%s = reader->readDouble(isSuccess);", field->m_name.c_str());
			}else if(field->m_typeId != 0) {
				writer.writeLine("isSuccess = value->%s.decode(proto, reader);", field->m_name.c_str());
			}
			writer.writeLine("break;");
			writer.removeIndent();
			writer.writeLine("}");
		}
		writer.removeIndent();
		writer.addIndent();
		writer.writeLine("default:");
		writer.addIndent();
		writer.writeLine("break;");
		writer.removeIndent();
		writer.writeLine("}");
		writer.writeLine("return isSuccess;");
	} else {
		writer.writeLine("return true;");
	}
	writer.removeIndent();
	writer.writeLine("}");
	writer.writeLine("");

	// start encode
	writer.writeLine("static bool on%sEncode(JMsgProto* proto, JMsgField* field, JMsgWriter* writer, void* args) {", type->m_typeName.c_str());
	writer.addIndent();
	
	if(type->m_vecFields.size() != 0) {
		writer.writeLine("%s* value = (%s*)args;", type->m_typeName.c_str(),  type->m_typeName.c_str());
		writer.writeLine("switch(field->m_id) {");
		for(size_t i = 0; i < type->m_vecFields.size(); i++) {

			JMsgField* field = type->m_vecFields[i];
			printf("generating field:%s\n", field->m_name.c_str());
			writer.writeLine("case %d: {", field->m_id);
			writer.addIndent();
			if(field->m_isArray) {
				writer.writeLine("int arrayLen = (int)value->%s.size();", field->m_name.c_str());
				writer.writeLine("writer->writeArrayHeader(field, arrayLen);");
				writer.writeLine("for(int i = 0; i < arrayLen; i++) {");
				writer.addIndent();
				 if(field->m_type == "string") {
					writer.writeLine("writer->writeString(value->%s[i]);", field->m_name.c_str());
				} else if(field->m_type == "int") {
					writer.writeLine("writer->writeInt(value->%s[i]);", field->m_name.c_str());
				} else if(field->m_type == "bool") {
					writer.writeLine("writer->writeBool(value->%s[i]);", field->m_name.c_str());
				} else if(field->m_type == "double"){
					writer.writeLine("writer->writeDouble(value->%s[i]);", field->m_name.c_str());
				} else if(field->m_typeId != 0) {
					writer.writeLine("value->%s[i].encode(proto, writer);", field->m_name.c_str());
				}
				writer.removeIndent();
				writer.writeLine("}");
			}  else if(field->m_type == "string") {
				writer.writeLine("writer->writeStringField(field, value->%s);", field->m_name.c_str());
			} else if(field->m_type == "bool") {
				writer.writeLine("writer->writeBoolField(field, value->%s);", field->m_name.c_str());
			} else if(field->m_type == "int") {
				writer.writeLine("writer->writeIntField(field, value->%s);", field->m_name.c_str());
			} else if(field->m_type == "double") {
				writer.writeLine("writer->writeDoubleField(field, value->%s);", field->m_name.c_str());
			} else if(field->m_typeId != 0) {
				writer.writeLine("writer->writeFieldHeader(field);", field->m_name.c_str());
				writer.writeLine("value->%s.encode(proto, writer);", field->m_name.c_str());
			}
			writer.writeLine("break;");
			writer.removeIndent();
			writer.writeLine("}");
		}
		writer.removeIndent();
		writer.addIndent();
		writer.writeLine("default:");
		writer.addIndent();
		writer.writeLine("break;");
		writer.removeIndent();
		writer.writeLine("}");
	}
	writer.writeLine("return true;");
	writer.removeIndent();
	writer.writeLine("}");

	writer.writeLine("void %s::encode(JMsgProto* proto, JMsgWriter* writer) {", type->m_typeName.c_str());
	writer.addIndent();
	writer.writeLine("proto->encode(%d, writer, on%sEncode, this);", type->m_id, type->m_typeName.c_str());
	writer.removeIndent();
	writer.writeLine("}");

	writer.writeLine("bool %s::decode(JMsgProto* proto, JMsgReader* reader) {", type->m_typeName.c_str());
	writer.addIndent();
	writer.writeLine("bool success = true;");
	writer.writeLine("m_msgId = reader->peekMessageTypeId(success);");
	writer.writeLine("return proto->decode(reader, on%sDecode, this) == %d;", type->m_typeName.c_str(), type->m_id);
	writer.removeIndent();
	writer.writeLine("}");

}

void writeHeaderCollect(const string& baseDir, const string& headerName, JMsgProto* protos) {
	JMSGCodeWriter writer;
	string filePath = baseDir + headerName + ".h";
	
	if(!writer.open(filePath)) {
		return;
	}

	writer.writeLine("#ifndef %s_h", headerName.c_str());
	writer.writeLine("#define %s_h", headerName.c_str());
	std::vector<JMsgType*>& types = protos->getAllTypes();

	for(size_t i = 0; i < types.size(); i++) {
		writer.writeLine("#include \"%s.h\"", types[i]->m_typeName.c_str());
	}
	writer.writeLine("#endif");
}

int main(int argc, char** argv) {
	if(argc != 4) {
		printf("useage:jmsg_generator ${config_file_name} $(output_path) $(all_header_name)");
		return -1;
	}
	
	lua_State* L = luaL_newstate();
	if(luaL_dofile(L, argv[1]) != 0) {
		lua_close(L);
		printf("lua exec error!\n");
		return -1;
	}
	
	//string* pcontent = jMsgGetFileString(argv[1]);
	//const char* content = pcontent->c_str();
	const char* content = luaL_checkstring(L, -1);
	printf("creating proto\n");
	JMsgProto* proto = JMsgProto::createProto(content);
	
	if(!proto) {
		//lua_close(L);
		printf("create proto failed\n");
		return -1;
	}
	std::vector<JMsgType*>& types = proto->getAllTypes();

	JMSGCodeWriter headerWriter;
	

	headerWriter.open(string(argv[2]) + argv[3] + ".h");
	headerWriter.writeLine("#ifndef %s_h",  argv[3]);
	headerWriter.writeLine("#define %s_h",  argv[3]);
	headerWriter.writeLine("#include <stdio.h>");
	headerWriter.writeLine("#include <vector>");
	headerWriter.writeLine("#include <string>");
	headerWriter.writeLine("#include \"jmsg_encodeable.h\"");
	headerWriter.writeLine("using namespace std;");
	headerWriter.writeLine("class JMsgWriter;");
	headerWriter.writeLine("class JMsgReader;");
	headerWriter.writeLine("class JMsgProto;");
	headerWriter.writeLine("enum %sTypeIds {", argv[3]);
	headerWriter.addIndent();
	for(int i = 0; i < types.size(); i++) {
		headerWriter.writeLine("k%s = %d,", types[i]->m_typeName.c_str(), types[i]->m_id);
	};
	headerWriter.removeIndent();
	headerWriter.writeLine("};");
	headerWriter.writeLine("");
	headerWriter.writeLine("JMsgProto* %sCreateProto();", argv[3]);
	JMSGCodeWriter cppWriter;
	cppWriter.open(string(argv[2]) + argv[3] + ".cpp");
	cppWriter.writeLine("#include \"%s.h\"", argv[3]);
	cppWriter.writeLine("#include \"jmsg.h\"");

	cppWriter.write("static const unsigned char s_protoString []= {");
	for(int i = 0; i < strlen(content); i++) {
		if(i % 10 == 0) {
			cppWriter.write("\n   ");
		}
		cppWriter.write("0x%02x,", (unsigned char)content[i]);		
	}
	cppWriter.write("0x00");
	cppWriter.write("\n");
	cppWriter.writeLine("};");
	cppWriter.writeLine("");
	cppWriter.writeLine("JMsgProto* %sCreateProto() { return JMsgProto::createProto((char*)s_protoString); }", argv[3]);
	cppWriter.writeLine("");
	for(size_t i = 0; i < types.size(); i++) {
		writeClassDeclare(argv[2], types[i], headerWriter);
		writeClassImplement(argv[2], types[i], cppWriter);
	}
	headerWriter.writeLine("#endif");
	
	//delete pcontent;
	lua_close(L);
}