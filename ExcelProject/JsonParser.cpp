#include "JsonParser.h"
#include <filesystem>

char* JsonParser::readFile(const char * fileName, int * readSize)
{
	FILE *fp = fopen(fileName, "rb");
	if (fp == nullptr)
		return nullptr;

	int size;
	char *buffer;

	// ���� ũ�� ���ϱ�
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// ���� ũ�� + NULL ������ŭ �޸𸮸� �Ҵ��ϰ� 0���� �ʱ�ȭ
	buffer = new char[size + 1];
	memset(buffer, 0, size + 1);

	if (fread(buffer, size, 1, fp) < 1)
	{
		*readSize = 0;
		delete buffer;
		fclose(fp);
		return nullptr;
	}

	// ���� ũ�⸦ �Ѱ���
	*readSize = size;

	fclose(fp);

	return buffer;
}

void JsonParser::ParseJson(char * doc, int size, JSON * json)
{
	int tokenIndex = 0;
	int pos = 0;

	if (doc[pos] != '{')
		return;

	pos++;

	while (pos < size)
	{
		char* begin;
		char* end;
		int stringLength;

		switch (doc[pos])
		{
		case '"':
		
			// ���ڿ��� ���� ��ġ ����. �� ���� "�� �����ϱ� ���� + 1
			begin = doc + pos + 1;

			// ���ڿ��� �� ��ġ�� ����. ���� " �� ��ġ
			end = strchr(begin, '"');
			if (end == nullptr)
				break;

			stringLength = end - begin;		// ���������� ���� ���̴� �� - ���� ��ġ

			// ��ū �迭�� ���ڿ� ����
			// ��ū ������ ���ڿ�
			json->tokens[tokenIndex].type = TOKEN_STRING;
			// ���ڿ� ���� + NULL ������ŭ �޸� �Ҵ�.
			json->tokens[tokenIndex].string = new char[stringLength + 1];
			memset(json->tokens[tokenIndex].string, 0, stringLength + 1);

			// �������� ���ڿ��� ��ū�� ����
			// ���ڿ� ���� ��ġ���� ���ڿ� ���̸�ŭ�� ����
			memcpy(json->tokens[tokenIndex].string, begin, stringLength);

			tokenIndex++;

			pos = pos + stringLength + 1;	// ���� ��ġ + ���ڿ� ���� + "( + 1)
			break;
		case '[':
		
			pos++;
			while (doc[pos] != ']')
			{
				if (doc[pos] == '"')
				{
					begin = doc + pos + 1;
					end = strchr(begin, '"');
					if (end == nullptr)
						break;

					stringLength = end - begin;

					json->tokens[tokenIndex].type = TOKEN_STRING;
					json->tokens[tokenIndex].string = new char[stringLength + 1];
					json->tokens[tokenIndex].isArray = true;
					memset(json->tokens[tokenIndex].string, 0, stringLength + 1);

					memcpy(json->tokens[tokenIndex].string, begin, stringLength);

					tokenIndex++;

					pos = pos + stringLength + 1;
				}
				pos++;
			}	
			break;

		case '0': case '1': case '2': case '3': case '4': case '5':
		case '6': case '7': case '8': case '9': case '-':
			begin = doc + pos;
			char* buffer;

			end = strchr(doc + pos, ',');
			if (end == nullptr)
			{
				end = strchr(doc + pos, '}');
				if (end == nullptr)
					break;
			}

			stringLength = end - begin;

			buffer = new char[stringLength + 1];
			memset(buffer, 0, stringLength + 1);
			memcpy(buffer, begin, stringLength);

			json->tokens[tokenIndex].type = TOKEN_NUMBER;
			json->tokens[tokenIndex].number = atof(buffer);

			delete buffer;
			tokenIndex++;

			pos = pos + stringLength + 1;
			break;
		}

		pos++;
	}
}

void JsonParser::FreeJson(JSON * json)
{
	for (int i = 0; i < TOKEN_COUNT; i++)
	{
		if (json->tokens[i].type == TOKEN_STRING)
			delete json->tokens[i].string;
	}
}

char * JsonParser::GetString(JSON * json, const char * key)
{
	for (int i = 0; i < TOKEN_COUNT; i++)
	{
		// ��ū ������ ���ڿ��̸鼭 ��ū�� ���ڿ��� Ű�� ��ġ�ϸ�
		if (json->tokens[i].type == TOKEN_STRING &&
			strcmp(json->tokens[i].string, key) == 0)
		{
			// �ٷ� ���� ��ū(i+1)�� ���ڿ��̸�
			if (json->tokens[i + 1].type == TOKEN_STRING)
				return json->tokens[i + 1].string;
		}
	}

	return nullptr;
}

char * JsonParser::GetArrayString(JSON * json, const char * key, int index)
{
	for (int i = 0; i < TOKEN_COUNT; i++)
	{
		if (json->tokens[i].type == TOKEN_STRING &&
			strcmp(json->tokens[i].string, key) == 0)
		{
			// �ٷ� ���� ��ū (i + 1)���� �迭�� ���
			// �ε����� ������ ��ū�� ���ڿ��̸鼭 �迭�̸�
			if (json->tokens[i + 1 + index].type == TOKEN_STRING &&
				json->tokens[i + 1 + index].isArray == true)
			{
				return json->tokens[i + 1 + index].string;
			}
		}
	}

	return nullptr;
}

int JsonParser::GetArrayCount(JSON * json, const char * key)
{
	for (int i = 0; i < TOKEN_COUNT; i++)
	{
		if (json->tokens[i].type == TOKEN_STRING &&
			strcmp(json->tokens[i].string, key) == 0)
		{
			int j = 0;
			while (json->tokens[i + 1 + j].isArray == true)
				j++;

			return j;
		}
	}
	return 0;
}

double JsonParser::GetNumber(JSON * json, const char * key)
{
	for (int i = 0; i < TOKEN_COUNT; i++)
	{
		if (json->tokens[i].type == TOKEN_STRING &&
			strcmp(json->tokens[i].string, key) == 0)
		{
			if (json->tokens[i + 1].type == TOKEN_NUMBER)
				return json->tokens[i + 1].number;
		}
	}

	return 0.0;
}

void JsonParser::WriteJsonFile()
{
	// JSON ������ ������ ������
	char *title = "Inception";
	int year = 2010;
	int runtime = 148;
	char *genre = "Sci-Fi";
	char *director = "Christopher Nolan";
	char actors[5][30] = {
		"Leonardo DiCaprio",
		"Joseph Gordon-Levitt",
		"Ellen Page",
		"Tom Hardy",
		"Ken Watanabe"
	};
	double imdbRating = 8.8;

	FILE *fp = fopen("example.json", "w");

	// JSON ������ ���缭 fprintf �Լ��� �� ���
	fprintf(fp, "{\n");
	fprintf(fp, "  \"Title\": \"%s\",\n", title);
	fprintf(fp, "  \"Year\": %d,\n", year);
	fprintf(fp, "  \"Runtime\": %d,\n", runtime);
	fprintf(fp, "  \"Genre\": \"%s\",\n", genre);
	fprintf(fp, "  \"Director\": \"%s\",\n", director);
	fprintf(fp, "  \"Actors\": [\n");
	fprintf(fp, "    \"%s\", \n", actors[0]);
	fprintf(fp, "    \"%s\", \n", actors[1]);
	fprintf(fp, "    \"%s\", \n", actors[2]);
	fprintf(fp, "    \"%s\", \n", actors[3]);
	fprintf(fp, "    \"%s\" \n", actors[4]);
	fprintf(fp, "  ],\n");
	fprintf(fp, "  \"imdbRating\": %.1f\n", imdbRating);
	fprintf(fp, "}\n");

	fclose(fp);
}

void JsonParser::TestCode()
{
	int size;
	char* doc = readFile("example.json", &size);
	if (doc == nullptr)
		return;

	JSON json;

	ParseJson(doc, size, &json);

	printf("Title: %s\n", GetString(&json, "Title"));        // ��ū�� ����� ���ڿ� ���(Title)
	printf("Year: %d\n", (int)GetNumber(&json, "Year"));    // ��ū�� ����� ���� ���(Year)
	printf("Runtime: %d\n", (int)GetNumber(&json, "Runtime")); // ��ū�� ����� ���� ���(Runtime)
	printf("Genre: %s\n", GetString(&json, "Genre"));        // ��ū�� ����� ���ڿ� ���(Genre)
	printf("Director: %s\n", GetString(&json, "Director"));     // ��ū�� ����� ���ڿ� ���(Director)
	printf("Actors:\n");
	int actors = GetArrayCount(&json, "Actors");
	for (int i = 0; i < actors; i++)
	{
		printf("%s\n", GetArrayString(&json, "Actors", i));
	}
	
	printf("imdbRating: %f\n", GetNumber(&json, "imdbRating")); // imdbRating�� �� ���
	FreeJson(&json);

	delete doc;
}
