#include <map>
#include <set>
#include <stdio.h>
#include "jmsg_idl_parse.h"
#include "jmsg_type.h"
#include "jmsg_field.h"
#include "jmsg_util.h"

using namespace std;

static char* skipEmptyChars(char* data) {
	while(jMsgIsEmptyChar(*data)) {
		data ++;
	}
	return data;
}

static char* skipComment(char* data) {
	if(*data == ';') {
		data ++;
		while(!jMsgIsChangeLine(*data)) {
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
		return NULL;
	}
}

static char* getcColons(char* data) {
	if(*data == ':') {
		data++;
		return data;
	} else {
		return NULL;
	}
}

static char* getCommonWord(char* data, string& word) {
	if(!jMsgIsAlpha(*data) && !jMsgIsUnderLine(*data)) {
		return NULL;
	}

	word.append(data, 1);
	data++;
	while(jMsgIsAlpha(*data) || jMsgIsDigit(*data) || jMsgIsUnderLine(*data)) {
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
	while(jMsgIsDigit(*data)) {
		ret = ret * 10 + (*data) - '0';
		found = true;
		data++;
	}

	if(found) {
		*number = ret;
		return data;
	} else {
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
	printf("read field name:%s\n", fieldName.c_str());
	data = skipEmptyChars(data);
	data = getcColons(data);

	if(!data) {
		return NULL;
	}

	data = skipEmptyChars(data);

	char* squireBranceStart = getSquareBrackets(data);
	if(squireBranceStart) {
		printf("squre bracket found\n");
		isArray = true;
		data = squireBranceStart;
	}


	data = getCommonWord(data, fieldType);
	printf("read field type:%s\n", fieldType.c_str());
	if(!data) {
		return NULL;
	}

	data = skipEmptyChars(data);

	data = skipComment(data);

	data = skipEmptyChars(data);

	data = getEqual(data);
	if(!data) {
		return NULL;
	}

	data = skipEmptyChars(data);

	data = getNumber(data, &fieldId);
	if(!data) {
		return NULL;
	}

	data = skipEmptyChars(data);

	field = new JMsgField;
	field->m_name = fieldName;
	field->m_type = fieldType;
	field->m_isArray = isArray;
	field->m_id = fieldId;
	*pField = field;

	data = skipComment(data);
	return data;
}

static char* getType(char* data, JMsgType** ppMsgType) {
	string typeName;
	int id;
	data = skipComment(data);
	data = getCommonWord(data, typeName);
	printf("gettype:%s\n", typeName.c_str());
	if(!data) {
		return NULL;
	}
	data = skipEmptyChars(data);
	data = skipComment(data);
	data = getEqual(data);
	if(!data) {
		return NULL;
	}

	data = skipEmptyChars(data);
	data = getNumber(data, &id);
	if(NULL == data) {
		return NULL;
	}
	printf("get type id=%d\n", id);
	data = skipEmptyChars(data);
	 data = getLeftBrace(data);
	if(NULL == data) {
		return NULL;
	}

	data = skipEmptyChars(data);
	data = skipComment(data);
	JMsgType* msgType = new JMsgType;
	msgType->m_typeName = typeName;
	msgType->m_id = id;
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
	   fieldName == "int" || 
	   fieldName == "double" || 
	   fieldName == "bool") {
		return true;
	}
	return false;
}


static bool checkMessages(std::vector<JMsgType*>& vecMessages) {
	map<string, JMsgType*> mapTypeNames;
	set<int> setTypeIds;
	printf("msg count=%d\n", vecMessages.size());
	for(size_t i = 0; i < vecMessages.size(); i++) {
		string& typeName = vecMessages[i]->m_typeName;
		printf("adding %s\n", typeName.c_str());
		if(mapTypeNames.find(typeName) == mapTypeNames.end()) {
			mapTypeNames[typeName] = vecMessages[i];
		} else {
			return false;
		}


		if(setTypeIds.find(vecMessages[i]->m_id) == setTypeIds.end()) {
			setTypeIds.insert(vecMessages[i]->m_id);
		} else {
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
					return false;
				} else {

					field->m_typeId =  iter->second->m_id;
					printf("set field typeid:%d\n", field->m_typeId);
				}
			}

			if(setFieldIds.find(field->m_id) == setFieldIds.end()) {
				setFieldIds.insert(field->m_id);
			} else {
				return false;
			}
		}
	}
	return true;
}

bool jMsgIDLParse(const string& strData, std::vector<JMsgType*>& vecMessages) {
	char* data = (char*)strData.c_str();

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