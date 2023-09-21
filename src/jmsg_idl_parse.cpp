#include <map>
#include <set>
#include <stdio.h>
#include "jmsg_idl_parse.h"
#include "jmsg_type.h"
#include "jmsg_field.h"
#include "jmsg_util.h"

using namespace std;
static char* s_startPos = nullptr;
static void jMsgOrderTypes(std::map<string, JMsgType*>& mapMessages, std::vector<JMsgType*>& vecMessages);

static string getErrorPos(const char* ptr) {
	int rowCount = 0;
	int colCount = 0;
	char* curPos = s_startPos;
	bool found = false;
	for (;;) {
		if (curPos == nullptr) {
			break;
		}

		if (curPos == ptr) {
			found = true;
			break;
		}

		if (*curPos == '\n') {
			rowCount++;
			colCount = 0;
		}
		else {
			colCount++;
		}
		curPos++;
	}

	return JMsgGetFormatString("row %d, col %d", rowCount, colCount);

}
static char* skipEmptyChars(char* data) {
	char* temp = data;
	while(JMsgIsEmptyChar(*data)) {
		data ++;
	}
	return data;
}

static char* skipComment(char* data) {
	char* temp = data;
	while(data && *data == ';') {
		data ++;
		while(!JMsgIsChangeLine(*data)) {
			data ++;
		}
		
		if(data) {
			data = skipEmptyChars(data);
		}
		
	} 

	
	return data;
}

static char* getLeftBrace(char* data) {
	if(*data == '{') {
		data++;
		return data;
	} else {
		printf("%s:getLeftBrace failed", getErrorPos(data).c_str());
		return NULL;
	}
}

static char* getcColons(char* data) {
	if(*data == ':') {
		data++;
		return data;
	} else {

		printf("%s:getcColons failed\n", getErrorPos(data).c_str());
		return NULL;
	}
}

static char* getCommonWord(char* data, string& word) {
	if(!JMsgIsAlpha(*data) && !JMsgIsUnderLine(*data)) {
		printf("%s:getCommonWord failed\n", getErrorPos(data).c_str());
		return NULL;
	}

	word.append(data, 1);
	data++;
	while(JMsgIsAlpha(*data) || JMsgIsDigit(*data) || JMsgIsUnderLine(*data)) {
		word.append(data, 1);
		data++;
	}
	return data;
}

static char* getEqual(char* data) {
	if(*data == '=') {
		data++;
		return data;
	} else {

		printf("%s:getEqual failed", getErrorPos(data).c_str());
		return NULL;
	}
}

static char* getSquareBrackets(char* data) {
	if(data[0] == '[' && data[1] == ']') {
		data += 2;
		return data;
	} else {
		return NULL;
	}
}

static char* getNumber(char* data, int* number) {
	int ret = 0;
	bool found = false;
	while(JMsgIsDigit(*data)) {
		ret = ret * 10 + (*data) - '0';
		found = true;
		data++;
	}

	if(found) {
		*number = ret;
		return data;
	} else {

		printf("%s:getNumber failed", getErrorPos(data).c_str());
		return NULL;
	}

}

static char* getField(char* data, JMsgField** pField) {
	string fieldName;
	string fieldType;
	int fieldId = 0;
	JMsgField* field = NULL;
	bool isArray = false;

	data = skipComment(data);
	data = getCommonWord(data, fieldName);
	
	if(!data) {
		return NULL;
	}
	data = skipEmptyChars(data);
	data = getcColons(data);

	if(!data) {
		return NULL;
	}

	data = skipEmptyChars(data);

	char* squireBranceStart = getSquareBrackets(data);
	if(squireBranceStart) {
		isArray = true;
		data = squireBranceStart;
	}


	data = getCommonWord(data, fieldType);
	if(!data) {
		return NULL;
	}

	data = skipEmptyChars(data);

	data = skipComment(data);

	data = skipEmptyChars(data);

	data = getEqual(data);
	if(!data) {
		//return NULL;
	}

	data = skipEmptyChars(data);

	data = getNumber(data, &fieldId);
	if(!data) {
		//return NULL;
	}

	data = skipEmptyChars(data);

	field = new JMsgField;
	field->m_name = fieldName;
	field->m_type = fieldType;
	field->m_isArray = isArray;
	//field->m_id = fieldId;
	*pField = field;

	data = skipComment(data);
	return data;
}

static char* getType(char* data, JMsgType** ppMsgType) {
	string typeName;
	int id;
	data = skipComment(data);
	data = getCommonWord(data, typeName);
	if(!data) {
		return NULL;
	}
	data = skipEmptyChars(data);
	data = skipComment(data);
	data = getEqual(data);
	if(!data) {
		//return NULL;
	}

	data = skipEmptyChars(data);
	data = getNumber(data, &id);
	if(NULL == data) {
		//return NULL;
	}
	data = skipEmptyChars(data);
	 data = getLeftBrace(data);
	if(NULL == data) {
		return NULL;
	}

	data = skipEmptyChars(data);
	data = skipComment(data);
	JMsgType* msgType = new JMsgType;
	msgType->m_typeName = typeName;
	
	while(*data != '}') {
		
		JMsgField* field = NULL;
		data = getField(data, &field);
		
		if(data) {
			msgType->m_vecFields.push_back(field);
		} else {
			break;
		}

	} 

	if(data) {
		data++;
		*ppMsgType = msgType;
	} else {
		delete msgType;
	}
	
	return data;
}

static bool isBasicType(const string& fieldName) {
	if(fieldName == "string" || 
	   fieldName == "int" || fieldName == "int64" ||
	   fieldName == "double" || 
	   fieldName == "bool") {
		return true;
	}
	return false;
}


static bool checkMessages(std::vector<JMsgType*>& vecMessages) {
	map<string, JMsgType*> mapTypeNames;
	set<int> setTypeIds;
	for(size_t i = 0; i < vecMessages.size(); i++) {
		string& typeName = vecMessages[i]->m_typeName;
		if(mapTypeNames.find(typeName) == mapTypeNames.end()) {
			mapTypeNames[typeName] = vecMessages[i];
		} else {
			printf("duplicate typeName:%s\n", typeName.c_str());
			return false;
		}
	}

	for(size_t i = 0; i < vecMessages.size(); i++) {
		JMsgType* msgType = vecMessages[i];
		set<string> setFieldNames;
		set<int> setFieldIds;
		for(size_t j = 0; j < msgType->m_vecFields.size(); j++) {
			JMsgField* field = msgType->m_vecFields[j];

			if(!isBasicType(field->m_type)) {
				map<string, JMsgType*>::iterator iter = mapTypeNames.find(field->m_type);
				if(iter == mapTypeNames.end()) {
                    printf("error:type %s not exist!\n", field->m_type.c_str());
					return false;
				}
			}
		}
	}
	jMsgOrderTypes(mapTypeNames, vecMessages);
	return true;
}

static bool jMsgTypeNameInVector(std::string& typeName, std::vector<JMsgType*>& vecMessages) {
	
	for(size_t i = 0; i < vecMessages.size(); i++) {
		if(vecMessages[i]->m_typeName == typeName) {
			return true;
		}
	}
	return false;
}

static void jMsgAddTypeToVector(JMsgType* msgType, std::map<string, JMsgType*>& mapMessages, std::vector<JMsgType*>& vecMessages) {
	if(jMsgTypeNameInVector(msgType->m_typeName, vecMessages)) {
		return;
	}

	for(size_t i = 0; i < msgType->m_vecFields.size(); i++) {
		JMsgField* field = msgType->m_vecFields[i];

		if(isBasicType(field->m_type)) {
			continue;
		}
		JMsgType* subType = mapMessages[field->m_type];
		jMsgAddTypeToVector(subType, mapMessages, vecMessages);		
	}
	vecMessages.push_back(msgType);
}

static void jMsgOrderTypes(std::map<string, JMsgType*>& mapMessages, std::vector<JMsgType*>& vecMessages) {
	vecMessages.clear();
	for(std::map<string, JMsgType*>::iterator iter = mapMessages.begin(); iter != mapMessages.end(); iter++) {
		jMsgAddTypeToVector(iter->second, mapMessages, vecMessages);
	}
}

bool jMsgIDLParse(const string& strData, std::vector<JMsgType*>& vecMessages) {
	char* data = (char*)strData.c_str();
	s_startPos = data;
	do {
		JMsgType* msgType = NULL;
		data = skipEmptyChars(data);
		data = skipComment(data);
		data = getType(data, &msgType);
		
		if(data) {
			vecMessages.push_back(msgType);
		} else {
			break;
		}
		data = skipEmptyChars(data);
	} while(*data != '\0');

	if(data) {
		return checkMessages(vecMessages);
	} else {
		return false;
	}
}