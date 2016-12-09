#include "jmsg.h"
#include "jmsg_util.h"
#include "jmsg_code_writer.h"
#include <string>
using namespace std;

void writeClassDeclare(const string& baseDir, JMsgType* type) {
	JMSGCodeWriter writer;
	string fileName = baseDir + type->m_typeName + ".h";
	if(!writer.open(fileName)) {
		printf("open %s failed\n", fileName.c_str());
		return;
	}
	writer.writeLine("#include <stdio.h>");
	writer.writeLine("#include <vector>");
	writer.writeLine("#include <string>");
	writer.writeLine("using namespace std;");
	writer.writeLine("class JMsgWriter;");
	writer.writeLine("class JMsgReader;");
	writer.writeLine("class JMsgProto;");
	writer.writeLine("struct %s {", type->m_typeName.c_str());
	writer.addIndent();

	for(int i = 0; i < type->m_vecFields.size(); i++) {
		JMsgField* field = type->m_vecFields[i];
		if(!field->m_isArray) {
			writer.writeLine("%s %s;", field->m_type.c_str(), field->m_name.c_str());
		} else {
			writer.writeLine("std::vector<%s> %s;", field->m_type.c_str(), field->m_name.c_str());
		}
	}

	writer.writeLine("void encode(JMsgProto* proto, JMsgWriter* writer);");
	writer.writeLine("void decode(JMsgProto* proto, JMsgReader* reader);");
	writer.removeIndent();
	writer.writeLine("}");
}

void writeClassImplement(const string& baseDir, JMsgType* type) {
	JMSGCodeWriter writer;
	string fileName = baseDir + type->m_typeName + ".cpp";
	if(!writer.open(fileName)) {
		printf("open %s failed\n", fileName.c_str());
		return;
	}
	writer.writeLine("#include \"%s.h\"", type->m_typeName.c_str());
	writer.writeLine("#include \"jmsg.h\"");
	writer.writeLine("");


	// start decode
	writer.writeLine("static void on%sDecode(JMsgProto* proto, JMsgField* field, JMsgReader* reader, void* args) {", type->m_typeName.c_str());
	writer.addIndent();
	writer.writeLine("%s* value = (%s*)args;", type->m_typeName.c_str(),  type->m_typeName.c_str());
	writer.writeLine("switch(field->m_id) {");
	for(int i = 0; i < type->m_vecFields.size(); i++) {

		JMsgField* field = type->m_vecFields[i];
		printf("generating field:%s\n", field->m_name.c_str());
		writer.writeLine("case %d: {", field->m_id);
		writer.addIndent();
		if(field->m_isArray) {
			writer.writeLine("int arraryLen = reader->readArrayLength();");
			writer.writeLine("for(int i = 0; i < arrayLen; i++) {");
			writer.addIndent();
			if(field->m_typeId != 0) {
				writer.writeLine("%s item;", field->m_type.c_str());
				writer.writeLine("item.decode(reader);");
				writer.writeLine("value.%s.push_back(item);", field->m_name.c_str());
			} else if(field->m_type == "string") {
				writer.writeLine("value.%s.push_back(reader->readString());", field->m_name.c_str());
			} else if(field->m_type == "int") {
				writer.writeLine("value.%s.push_back(reader->readInt());", field->m_name.c_str());
			} else if(field->m_type == "bool") {
				writer.writeLine("value.%s.push_back(reader->readBool());", field->m_name.c_str());
			} else {
				writer.writeLine("value.%s.push_back(reader->readDouble());", field->m_name.c_str());
			}
			writer.removeIndent();
			writer.writeLine("}");
		} else if(field->m_typeId != 0) {
			writer.writeLine("value.%s.decode(reader);", field->m_name.c_str());
		} else if(field->m_type == "string") {
			writer.writeLine("value.%s = reader->readString()", field->m_name.c_str());
		} else if(field->m_type == "bool") {
			writer.writeLine("value.%s = reader->readBool()", field->m_name.c_str());
		} else if(field->m_type == "int") {
			writer.writeLine("value.%s = reader->readInt()", field->m_name.c_str());
		} else if(field->m_type == "double") {
			writer.writeLine("value.%s = reader->readDouble()", field->m_name.c_str());
		}
		writer.writeLine("break;");
		writer.removeIndent();
		writer.writeLine("}");
	}
	writer.removeIndent();
	writer.addIndent();
	writer.writeLine("default:");
	writer.addIndent();
	writer.writeLine("break");
	writer.removeIndent();
	writer.writeLine("}");
	writer.removeIndent();
	writer.writeLine("}");
	writer.writeLine("");

	// start encode
	writer.writeLine("static void on%sEncode(JMsgProto* proto, JMsgField* field, JMsgWriter* writer, void* args) {", type->m_typeName.c_str());
	writer.addIndent();
	writer.writeLine("%s* value = (%s*)args;", type->m_typeName.c_str(),  type->m_typeName.c_str());
	writer.writeLine("switch(field->m_id) {");
	for(int i = 0; i < type->m_vecFields.size(); i++) {

		JMsgField* field = type->m_vecFields[i];
		printf("generating field:%s\n", field->m_name.c_str());
		writer.writeLine("case %d: {", field->m_id);
		writer.addIndent();
		if(field->m_isArray) {
			writer.writeLine("int arraryLen = reader->readArrayLength();");
			writer.writeLine("for(int i = 0; i < arrayLen; i++) {");
			writer.addIndent();
			if(field->m_typeId != 0) {
				writer.writeLine("value.%s[i].encode(proto, writer);", field->m_name.c_str());
			} else if(field->m_type == "string") {
				writer.writeLine("writer.writeString(value.%s[i]);", field->m_name.c_str());
			} else if(field->m_type == "int") {
				writer.writeLine("writer.writeInt(value.%s[i]);", field->m_name.c_str());
			} else if(field->m_type == "bool") {
				writer.writeLine("writer.writeBool(value.%s[i]);", field->m_name.c_str());
			} else {
				writer.writeLine("writer.writeDouble(value.%s[i]);", field->m_name.c_str());
			}
			writer.removeIndent();
			writer.writeLine("}");
		} else if(field->m_typeId != 0) {
			writer.writeLine("value.%s.encode(proto, writer);", field->m_name.c_str());
		} else if(field->m_type == "string") {
			writer.writeLine("writer->writeStringField(field, value.%s[i])", field->m_name.c_str());
		} else if(field->m_type == "bool") {
			writer.writeLine("writer->writeBoolField(field, value.%s[i])", field->m_name.c_str());
		} else if(field->m_type == "int") {
			writer.writeLine("writer->writeIntField(field, value.%s[i])", field->m_name.c_str());
		} else if(field->m_type == "double") {
			writer.writeLine("writer->writeDoubleField(field, value.%s[i])", field->m_name.c_str());
		}
		writer.writeLine("break;");
		writer.removeIndent();
		writer.writeLine("}");
	}
	writer.removeIndent();
	writer.addIndent();
	writer.writeLine("default:");
	writer.addIndent();
	writer.writeLine("break");
	writer.removeIndent();
	writer.writeLine("}");
	writer.removeIndent();
	writer.writeLine("}");

	writer.writeLine("void %s::encode(JMsgProto* proto, JMsgWriter* writer) {", type->m_typeName.c_str());
	writer.addIndent();
	writer.writeLine("proto->encode(%d, writer, on%sEncode, this);", type->m_id, type->m_typeName.c_str());
	writer.removeIndent();
	writer.writeLine("}");

	writer.writeLine("void %s::decode(JMsgProto* proto, JMsgReader* reader) {", type->m_typeName.c_str());
	writer.addIndent();
	writer.writeLine("proto->decode(reader, on%sDecode, this);", type->m_typeName.c_str());
	writer.removeIndent();
	writer.writeLine("}");

}

int main(int argc, char** argv) {
	string* content = jMsgGetFileString("../test/config1.txt");
	JMsgProto* proto = JMsgProto::createProto(*content);
	delete content;

	if(!proto) {
		printf("create proto failed\n");
		return -1;
	}
	std::vector<JMsgType*>& types = proto->getAllTypes();

	for(int i = 0; i < types.size(); i++) {
		writeClassDeclare("../output/", types[i]);
		writeClassImplement("../output/", types[i]);
	}
}