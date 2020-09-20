#pragma once

#define _CRT_SECURE_NO_WARNINGS    // fopen 보안 경고로 인한 컴파일 에러 방지

#define TOKEN_COUNT 20

typedef enum _TOKEN_TYPE
{
	TOKEN_STRING,
	TOKEN_NUMBER,
} TOKEN_TYPE;

typedef struct _TOKEN
{
	TOKEN_TYPE type;
	union
	{
		char *string;
		double number;
	};
	bool isArray;
} TOKEN;

typedef struct _JSON
{
	TOKEN tokens[TOKEN_COUNT];
} JSON;

class JsonParser
{
public:
	char* readFile(const char *fileName, size_t* readSize);
	void ParseJson(char *doc, size_t size, JSON *json);
	void FreeJson(JSON* json);
	char* GetString(JSON* json, const char* key);
	char* GetArrayString(JSON* json, const char* key, int index);
	size_t GetArrayCount(JSON* json, const char* key);
	double GetNumber(JSON* json, const  char* key);
	void WriteJsonFile();
	void TestCode();
};

