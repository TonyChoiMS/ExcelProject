#include "JsonParser.h"
#include <filesystem>
#include <locale>
#include <string>
#include <iostream>

char* JsonParser::readFile(const char * fileName, size_t * readSize)
{
	FILE *fp = fopen(fileName, "rb");
	std::cout << fp << std::endl;
	if (fp == nullptr)
		return nullptr;

	size_t size;
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

void JsonParser::ParseJson(char * doc, size_t size, JSON * json)
{
	size_t tokenIndex = 0;
	size_t pos = 0;
	
	//if (doc[pos] != L'{')
	/*if (wcscmp(const_cast<wchar_t &>(doc[pos]), L'{'))
		return;*/

	pos++;
	
	while (pos < size)
	{
		char* begin;
		char* end;
		size_t stringLength;
		//std::wcout << "doc[pos] :" << doc[pos] << std::endl;
		switch (doc[pos])
		{
		case L'"':
		
			// ���ڿ��� ���� ��ġ ����. �� ���� "�� �����ϱ� ���� + 1
			begin = doc + pos + 1;

			// ���ڿ��� �� ��ġ�� ����. ���� " �� ��ġ
			end = strchr(begin, L'"');
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
		case L'[':
		
			pos++;
			while (doc[pos] != L']')
			{
				if (doc[pos] == L'"')
				{
					begin = doc + pos + 1;
					end = strchr(begin, L'"');
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

		case L'0': case L'1': case L'2': case L'3': case L'4': case L'5':
		case L'6': case L'7': case L'8': case L'9': case L'-':
			begin = doc + pos;
			char* buffer;

			end = strchr(doc + pos, L',');
			if (end == nullptr)
			{
				end = strchr(doc + pos, L'}');
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

char * JsonParser::GetString(JSON* json, const char* key)
{
	
	for (int i = 0; i < TOKEN_COUNT; i++)
	{
		//std::wcout << "json->tokens[i].type : " << json->tokens[i].type << std::endl;// "," << wcscmp(json->tokens[i].string, key) << ", " << json->tokens[i].string << std::endl;
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

size_t JsonParser::GetArrayCount(JSON * json, const char * key)
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
	const wchar_t *title = L"Inception";
	size_t year = 2010;
	size_t runtime = 148;
	const wchar_t *genre = L"Sci-Fi";
	const wchar_t *director = L"Christopher Nolan";
	const wchar_t actors[5][30] = {
		L"Leonardo DiCaprio",
		L"Joseph Gordon-Levitt",
		L"Ellen Page",
		L"Tom Hardy",
		L"Ken Watanabe"
	};
	double imdbRating = 8.8;

	FILE *fp = fopen("example.json", "w");

	// JSON ������ ���缭 fprintf �Լ��� �� ���
	fprintf(fp, "{\n");
	fprintf(fp, "  \"Title\": \"%ls\",\n", title);
	fprintf(fp, "  \"Year\": %ls,\n", year);
	fprintf(fp, "  \"Runtime\": %Ls,\n", runtime);
	fprintf(fp, "  \"Genre\": \"%ws\",\n", genre);
	fprintf(fp, "  \"Director\": \"%ls\",\n", director);
	fprintf(fp, "  \"Actors\": [\n");
	fprintf(fp, "    \"%ls\", \n", actors[0]);
	fprintf(fp, "    \"%ls\", \n", actors[1]);
	fprintf(fp, "    \"%ls\", \n", actors[2]);
	fprintf(fp, "    \"%ls\", \n", actors[3]);
	fprintf(fp, "    \"%ls\" \n", actors[4]);
	fprintf(fp, "  ],\n");
	fprintf(fp, "  \"imdbRating\": %.1f\n", imdbRating);
	fprintf(fp, "}\n");

	fclose(fp);
}

void JsonParser::TestCode()
{
	size_t size;
	char* doc = readFile("example.json", &size);
	if (doc == nullptr)
		return;

	JSON json;

	ParseJson(doc, size, &json);

	std::cout << "Title: " << GetString(&json, "Title") << std::endl;
	std::cout << "Year: " << (int)GetNumber(&json, "Year") << std::endl;
	std::cout << "Runtime: " << (int)GetNumber(&json, "Runtime") << std::endl;
	std::cout << "Genre: " << GetString(&json, "Genre") << std::endl;
	std::cout << "Director: " << GetString(&json, "Director") << std::endl;
	std::cout << "Actors: " << std::endl;
	
	size_t actors = GetArrayCount(&json, "Actors");
	for (size_t i = 0; i < actors; i++)
	{
		std::cout << GetArrayString(&json, "Actors", i) << std::endl;
	}
	
	std::cout << "imdbRating: " << GetNumber(&json, "imdbRating") << std::endl;
	
	FreeJson(&json);

	delete doc;
}
