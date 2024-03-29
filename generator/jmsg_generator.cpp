#include "jmsg.h"
#include "jmsg_util.h"
#include "jmsg_code_writer.h"
#include <string>
#include <set>
using namespace std;
enum GenerateType {
	kGenerateTypeBinary = 0,
	kGenerateTypeJson = 1,
	kGenerateTypeBoth = 2
};
GenerateType s_generateType = kGenerateTypeJson;

static bool isGenerateJson() {
	return s_generateType == kGenerateTypeBoth || s_generateType == kGenerateTypeJson;
}

void writeTypeDeclare(JMsgType* type, JMSGCodeWriter& writer) {
	writer.writeLine("class %s;", type->m_typeName.c_str());
}

void writeClassDeclare(const string& baseDir, JMsgType* type, JMSGCodeWriter& writer) {	
	writer.writeLine("class %s : public IJMsgEncodeable{", type->m_typeName.c_str());
	writer.writeLine("public:");
	writer.addIndent();
	writer.writeLine("%s();", type->m_typeName.c_str());
	for(size_t i = 0; i < type->m_vecFields.size(); i++) {
		JMsgField* field = type->m_vecFields[i];
        string typeName = field->m_type;
        if (field->m_type == "int64") {
            typeName = "int64_t";
        }
		if(!field->m_isArray) {           
			writer.writeLine("std::optional<%s> %s;", typeName.c_str(), field->m_name.c_str());
		} else {
			writer.writeLine("std::optional<std::vector<%s>> %s;", typeName.c_str(), field->m_name.c_str());
		}
	}	

    writer.writeLine("void encodeJson(rapidjson::Document& doc, rapidjson::Value& val) override;");
    writer.writeLine("bool decodeJson(rapidjson::Value& val) override;");
	writer.writeLine("std::string getMsgName() override {return \"%s\";}", type->m_typeName.c_str());
	
	writer.removeIndent();
	writer.writeLine("};");
	writer.writeLine("");
}

void writeClassImplement(const string& baseDir, JMsgType* type, JMSGCodeWriter& writer) {

	writer.writeLine("%s::%s() {", type->m_typeName.c_str(), type->m_typeName.c_str());
	writer.addIndent();
	//writer.writeLine("m_msgId = %d;", type->m_id);

	writer.removeIndent();
	writer.writeLine("}");
	writer.writeLine("");		
	
	if(isGenerateJson()) {
		// json encode/decode
		// start decode
		writer.writeLine("");
		writer.writeLine("static bool on%sDecodeJson(JMsgProto* proto, JMsgField* field, rapidjson::Value& jsonValue, void* args) {", type->m_typeName.c_str());
		writer.addIndent();
		if(type->m_vecFields.size() != 0) {
			writer.writeLine("bool isSuccess = false;");
			writer.writeLine("%s* value = (%s*)args;", type->m_typeName.c_str(),  type->m_typeName.c_str());
			bool firstIf = true;
			//writer.writeLine("switch(field->m_name) {");
			for(size_t i = 0; i < type->m_vecFields.size(); i++) {
				JMsgField* field = type->m_vecFields[i];
				printf("generating field:%s\n", field->m_name.c_str());
				if(firstIf) {
					writer.writeLine("if(field->m_name == \"%s\") {", field->m_name.c_str());
				} else {
					writer.writeLine("else if(field->m_name == \"%s\") {", field->m_name.c_str());
				}
				
				firstIf = false;
				writer.addIndent();

				if(field->m_isArray) {
                    string typeName = field->m_type;
                    if (field->m_type == "int64") {
                        typeName = "int64_t";
                    }
                    writer.writeLine("std::vector<%s> arrayValue;", typeName.c_str());
                    writer.writeLine("if(!jsonValue.IsArray()) return false;");
					writer.writeLine("for(size_t i = 0; i < jsonValue.Size(); i++) {");
					writer.addIndent();
					if(field->m_type == "string") {
						writer.writeLine("arrayValue.push_back(jsonValue[(int)i].IsString() ? jsonValue[(int)i].GetString() : \"\");");
					} else if(field->m_type == "int") {
						writer.writeLine("arrayValue.push_back(jsonValue[(int)i].IsInt() ? jsonValue[(int)i].GetInt() : 0);");
					}
                    else if (field->m_type == "int64") {
                        writer.writeLine("arrayValue.push_back(jsonValue[(int)i].IsInt64() ? jsonValue[(int)i].GetInt64() : 0);");
                    }
                    else if(field->m_type == "bool") {
						writer.writeLine("arrayValue.push_back(jsonValue[(int)i].IsBool() ? jsonValue[(int)i].GetBool() : false);");
					} else if(field->m_type == "double"){
						writer.writeLine("arrayValue.push_back(jsonValue[(int)i].IsDouble() ? jsonValue[(int)i].GetDouble(): 0);");
					} else if(field->m_type != "") {
						writer.writeLine("%s item;", field->m_type.c_str());
						writer.writeLine("isSuccess = item.decodeJson(jsonValue[(int)i]);");
						writer.writeLine("if(!isSuccess) break;");
						writer.writeLine("arrayValue.push_back(item);");
					} 
                    writer.writeLine("value->%s = arrayValue;", field->m_name.c_str());
					writer.removeIndent();
					writer.writeLine("}");
				} else if(field->m_type == "string") {
					writer.writeLine("if(jsonValue.IsString()) value->%s = jsonValue.GetString();", field->m_name.c_str());
				} else if(field->m_type == "bool") {
					writer.writeLine("if(jsonValue.IsBool()) value->%s = jsonValue.GetBool();", field->m_name.c_str());
				} else if(field->m_type == "int") {
					writer.writeLine("if(jsonValue.IsInt()) value->%s = jsonValue.GetInt();", field->m_name.c_str());
				}
                else if (field->m_type == "int64") {
                    writer.writeLine("if(jsonValue.IsInt64()) value->%s = jsonValue.GetInt64();", field->m_name.c_str());
                }
                else if(field->m_type == "double") {
					writer.writeLine("if(jsonValue.IsDouble()) value->%s = jsonValue.GetDouble();", field->m_name.c_str());
				}else if(field->m_type != "") {
                    writer.writeLine("if(!jsonValue.IsObject()) return false;");
                    writer.writeLine("%s decodedValue;", field->m_type.c_str());
					writer.writeLine("if(decodedValue.decodeJson(jsonValue)) {", field->m_name.c_str());
                    writer.addIndent();
                    writer.writeLine("value->%s = decodedValue;", field->m_name.c_str());
                    writer.removeIndent();
                    writer.writeLine("}");
				}
				writer.removeIndent();
				writer.writeLine("}");
			}
			writer.removeIndent();
			writer.addIndent();
			writer.writeLine("return isSuccess;");
		} else {
			writer.writeLine("return true;");
		}
		writer.removeIndent();
		writer.writeLine("}");
		writer.writeLine("");
		
		// start encode
		writer.writeLine("static bool on%sEncodeJson(JMsgProto* proto, JMsgField* field, rapidjson::Document& doc, rapidjson::Value& jsonValue, void* args) {", type->m_typeName.c_str());
		writer.addIndent();

		if(type->m_vecFields.size() != 0) {
            writer.writeLine("auto& allocator = doc.GetAllocator();");
			writer.writeLine("%s* value = (%s*)args;", type->m_typeName.c_str(),  type->m_typeName.c_str());
			bool firstIf = true;
			//writer.writeLine("switch(field->m_name) {");
			for(size_t i = 0; i < type->m_vecFields.size(); i++) {

				JMsgField* field = type->m_vecFields[i];
				printf("generating field:%s\n", field->m_name.c_str());
				if(firstIf) {
					writer.writeLine("if(field->m_name == \"%s\") {", field->m_name.c_str());
				} else {
					writer.writeLine("else if(field->m_name == \"%s\") {", field->m_name.c_str());
				}
				
				firstIf = false;
				writer.addIndent();
				if(field->m_isArray) {
                    writer.writeLine("if(value->%s.has_value()) {", field->m_name.c_str());
					writer.addIndent();
					writer.writeLine("rapidjson::Value arrayValue(rapidjson::kArrayType);");
					writer.writeLine("for(size_t i = 0; i < value->%s.value().size(); i++) {", field->m_name.c_str());
					writer.addIndent();
					if(field->m_type == "int" || field->m_type == "int64" || field->m_type == "bool" || field->m_type == "double") {
						writer.writeLine("arrayValue.PushBack(rapidjson::Value(value->%s.value()[i]).Move(), allocator);", field->m_name.c_str());
					} else if (field->m_type == "string") {
                        writer.writeLine("rapidjson::Value valStr;");
                        writer.writeLine("valStr.SetString(value->%s.value()[i].c_str(), (rapidjson::SizeType)value->%s.value()[i].size(), allocator);", field->m_name.c_str(), field->m_name.c_str());
                        writer.writeLine("arrayValue.PushBack(valStr.Move(), allocator);");
                    } else if(field->m_type != "") {
						writer.writeLine("rapidjson::Value itemValue(rapidjson::kObjectType);");
						writer.writeLine("value->%s.value()[i].encodeJson(doc, itemValue);", field->m_name.c_str());
						writer.writeLine("arrayValue.PushBack(itemValue.Move(), allocator);");
					}				
					writer.removeIndent();
					writer.writeLine("}");
					writer.writeLine("jsonValue.AddMember(\"%s\", arrayValue, allocator);", field->m_name.c_str());
					writer.removeIndent();
					writer.writeLine("}");
				} else if (field->m_type == "string") {
                    writer.writeLine("if(value->%s.has_value()) {", field->m_name.c_str());
					writer.addIndent();
                    writer.writeLine("rapidjson::Value valStr;");
                    writer.writeLine("valStr.SetString(value->%s.value().c_str(), (rapidjson::SizeType)value->%s.value().size(), allocator);", field->m_name.c_str(), field->m_name.c_str());
                    writer.writeLine("jsonValue.AddMember(\"%s\", valStr.Move(), allocator);", field->m_name.c_str());
					writer.removeIndent();
					writer.writeLine("}");
                }
                else if(field->m_type == "int" || field->m_type == "int64" || field->m_type == "bool" || field->m_type == "double") {
                    writer.writeLine("if(value->%s.has_value()){", field->m_name.c_str());
					writer.addIndent();
					writer.writeLine("jsonValue.AddMember(\"%s\", value->%s.value(), allocator);", field->m_name.c_str(), field->m_name.c_str(), field->m_name.c_str());
					writer.removeIndent();
					writer.writeLine("}");
				} else if(field->m_type != "") {
                    writer.writeLine("if(value->%s.has_value()) {", field->m_name.c_str());
					writer.writeLine("rapidjson::Value itemValue(rapidjson::kObjectType);");
					writer.writeLine("value->%s.value().encodeJson(doc, itemValue);", field->m_name.c_str());
					writer.writeLine("jsonValue.AddMember(\"%s\", itemValue.Move(), allocator);", field->m_name.c_str());
					writer.removeIndent();
					writer.writeLine("}");
				}
				writer.removeIndent();
				writer.writeLine("}");
			}
			writer.removeIndent();
			writer.addIndent();
		}
		writer.writeLine("return true;");
		writer.removeIndent();
		writer.writeLine("}");
		writer.writeLine("");
		writer.writeLine("void %s::encodeJson( rapidjson::Document& doc, rapidjson::Value& writer) {", type->m_typeName.c_str());
		writer.addIndent();
		writer.writeLine("g_proto->encodeJson(\"%s\", doc, writer, on%sEncodeJson, this);", type->m_typeName.c_str(), type->m_typeName.c_str());
		writer.removeIndent();
		writer.writeLine("}");
		writer.writeLine("");
		writer.writeLine("bool %s::decodeJson(rapidjson::Value& reader) {", type->m_typeName.c_str());
		writer.addIndent();
		writer.writeLine("return g_proto->decodeJson(\"%s\", reader, on%sDecodeJson, this);", type->m_typeName.c_str(), type->m_typeName.c_str());
		writer.removeIndent();
		writer.writeLine("}");
	}


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

static void generateGoJsonDefine(const string& outputFileName, std::vector<JMsgType*>& vecTypes) {
	JMSGCodeWriter goWritter;
	goWritter.open(outputFileName);

	goWritter.writeLine("package main");

	for(size_t i = 0; i < vecTypes.size(); i++) {
		JMsgType& msgType =*vecTypes[i];
		
		goWritter.writeLine("type %s struct {", msgType.m_typeName.c_str());
		goWritter.addIndent();
		for(size_t j = 0; j < msgType.m_vecFields.size(); j++) {
			JMsgField msgField = *msgType.m_vecFields[j];

			if(msgField.m_isArray) {
				goWritter.writeLine("%s []%s `json:\"%s\"`", JMsgGetFirstCharBigerCase(msgField.m_name).c_str(), msgField.m_type.c_str(), msgField.m_name.c_str());
			} else {
				goWritter.writeLine("%s %s `json:\"%s\"`", JMsgGetFirstCharBigerCase(msgField.m_name).c_str(), msgField.m_type.c_str(), msgField.m_name.c_str());
			}
		}
		goWritter.removeIndent();
		goWritter.writeLine("}");
		goWritter.writeLine("");		
	}
}

void generateHeaderFile(const std::string& outputPath, const string& prefix, std::vector<JMsgType*>& types, const string& msgNamespace) {
	JMSGCodeWriter headerWriter;		
	headerWriter.open(outputPath + prefix + ".h");
	headerWriter.writeLine("#ifndef %s_h",  prefix.c_str());
	headerWriter.writeLine("#define %s_h",  prefix.c_str());
	headerWriter.writeLine("#include <stdio.h>");
	headerWriter.writeLine("#include <vector>");
	headerWriter.writeLine("#include <string>");
    headerWriter.writeLine("#include <optional>");
	headerWriter.writeLine("#include \"jmsg_encodeable.h\"");
    headerWriter.writeLine("#include \"rapidjson/rapidjson.h\"");
    headerWriter.writeLine("#include \"rapidjson/document.h\"");
	headerWriter.writeLine("using namespace std;");
	headerWriter.writeLine("class JMsgProto;");
	if(msgNamespace != "") {
		headerWriter.writeLine("namespace %s {", msgNamespace.c_str());
	}
	
    headerWriter.writeLine("");
	headerWriter.writeLine("enum %sTypeIds {", prefix.c_str());
	headerWriter.addIndent();
	for(size_t i = 0; i < types.size(); i++) {
		headerWriter.writeLine("k%s,", types[i]->m_typeName.c_str());
	};
	headerWriter.removeIndent();
	headerWriter.writeLine("};");
	headerWriter.writeLine("");
	headerWriter.writeLine("JMsgProto* %sCreateProto();", prefix.c_str());
	headerWriter.writeLine("void %sInit();", prefix.c_str()); 
	headerWriter.writeLine("void %sFini();", prefix.c_str());
	headerWriter.writeLine("JMsgProto* %sGetProto();", prefix.c_str());

	for(size_t i = 0; i < types.size(); i++) {
		writeClassDeclare(outputPath, types[i], headerWriter);
	}

	if(msgNamespace != "") {
		headerWriter.writeLine("} // namespace %s", msgNamespace.c_str());
	}
	headerWriter.writeLine("#endif");
}

void generateCppFile(const std::string& outputPath, const string& prefix, std::vector<JMsgType*>& types, const string& content, const string& msgNamespace) {

	JMSGCodeWriter cppWriter;
	cppWriter.open(outputPath + prefix + ".cpp");
	cppWriter.writeLine("#include \"%s.h\"", prefix.c_str());
	cppWriter.writeLine("#include \"jmsg.h\"");
	if(msgNamespace != "") {
		cppWriter.writeLine("namespace %s {", msgNamespace.c_str());
	}

	cppWriter.write("static const unsigned char s_protoString []= {");
	for(size_t i = 0; i < content.size(); i++) {
		if(i % 10 == 0) {
			cppWriter.write("\n   ");
		}
		cppWriter.write("0x%02x,", (unsigned char)content[i]);		
	}
	cppWriter.write("0x00");
	cppWriter.write("\n");
	cppWriter.writeLine("};");
	cppWriter.writeLine("");
	cppWriter.writeLine("JMsgProto* %sCreateProto() { return JMsgProto::createProto((char*)s_protoString); }", prefix.c_str());
	cppWriter.writeLine("static JMsgProto* g_proto = NULL;");
	cppWriter.writeLine("void %sInit(){", prefix.c_str());
	cppWriter.addIndent();
	cppWriter.writeLine("g_proto = %sCreateProto();", prefix.c_str());
	cppWriter.removeIndent();
	cppWriter.writeLine("}");
	cppWriter.writeLine("void %sFini(){", prefix.c_str());
	cppWriter.addIndent();
	cppWriter.writeLine("if(g_proto) {");
	cppWriter.addIndent();
	cppWriter.writeLine("delete g_proto;");
	cppWriter.writeLine("g_proto = NULL;"); 
	cppWriter.removeIndent();
	cppWriter.writeLine("}");
	cppWriter.removeIndent();
	cppWriter.writeLine("}");
	cppWriter.writeLine("");
	cppWriter.writeLine("JMsgProto* %sGetProto() { return g_proto; }", prefix.c_str());
	cppWriter.writeLine("");
	for(size_t i = 0; i < types.size(); i++) {
		writeClassImplement(outputPath, types[i], cppWriter);
	}

	if(msgNamespace != "") {
		cppWriter.writeLine("} // namespace %s", msgNamespace.c_str());
	}
}

void generateDeclareFile(const std::string& outputPath, const string& prefix, std::vector<JMsgType*>& types, const std::string& msgNamespace) {
	JMSGCodeWriter declareWriter;
	declareWriter.open(outputPath + prefix + "Declare.h");
	declareWriter.writeLine("#ifndef %sDeclare_h",  prefix.c_str());
	declareWriter.writeLine("#define %sDeclare_h",  prefix.c_str());

	if(msgNamespace != "") {
		declareWriter.writeLine("namespace %s {", msgNamespace.c_str());
	}


	for(size_t i = 0; i < types.size(); i++) {
		writeTypeDeclare(types[i], declareWriter);
	}

	if(msgNamespace != "") {
		declareWriter.writeLine("} // namespace %s", msgNamespace.c_str());
	}
	declareWriter.writeLine("#endif");
}

int main(int argc, char** argv) {
	if(argc < 4) {
		printf("useage:jmsg_generator ${config_file_name} $(output_path) $(all_header_name) argc=%d\n", argc);
		getchar();
		return -1;
	}

    s_generateType = kGenerateTypeJson;

	string content;
	
	if(!JMsgGetFileString(argv[1], content)) {
		printf("read file %s failed\n", argv[1]);
		getchar();
		return - 1;
	}
	
	JMsgProto* proto = JMsgProto::createProto(content);
	
	if(!proto) {
		printf("create proto failed\n");
		getchar();
		return -1;
	}

	std::vector<JMsgType*>& types = proto->getAllTypes();
	string outputPath = argv[2];
	string prefix = argv[3];
	string msgNamespace;

	if(argc > 4) {
		msgNamespace = argv[4];
	}

	generateHeaderFile(outputPath, prefix, types, msgNamespace);
	generateCppFile(outputPath, prefix, types, content, msgNamespace);
	generateDeclareFile(outputPath, prefix, types, msgNamespace);
}