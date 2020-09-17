#pragma once
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

class JsonParser
{
};

