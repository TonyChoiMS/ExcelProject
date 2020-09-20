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
	
	// 파일 크기 구하기
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// 파일 크기 + NULL 공간만큼 메모리를 할당하고 0으로 초기화
	
	buffer = new char[size + 1];
	memset(buffer, 0, size + 1);

	if (fread(buffer, size, 1, fp) < 1)
	{
		*readSize = 0;
		delete buffer;
		fclose(fp);
		return nullptr;
	}

	// 파일 크기를 넘겨줌
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
		
			// 문자열의 시작 위치 구함. 맨 앞의 "를 제외하기 위해 + 1
			begin = doc + pos + 1;

			// 문자열의 끝 위치를 구함. 다음 " 의 위치
			end = strchr(begin, L'"');
			if (end == nullptr)
				break;

			stringLength = end - begin;		// 문자졍르이 실제 길이는 끝 - 시작 위치

			// 토큰 배열에 문자열 저장
			// 토큰 종류는 문자열
			json->tokens[tokenIndex].type = TOKEN_STRING;
			// 문자열 길이 + NULL 공간만큼 메모리 할당.
			json->tokens[tokenIndex].string = new char[stringLength + 1];
			memset(json->tokens[tokenIndex].string, 0, stringLength + 1);

			// 문서에서 문자열을 토큰에 저장
			// 문자열 시작 위치에서 문자열 길이만큼만 복사
			memcpy(json->tokens[tokenIndex].string, begin, stringLength);

			tokenIndex++;

			pos = pos + stringLength + 1;	// 현재 위치 + 문자열 길이 + "( + 1)
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
		// 토큰 종류가 문자열이면서 토큰의 문자열이 키와 일치하면
		if (json->tokens[i].type == TOKEN_STRING &&
			strcmp(json->tokens[i].string, key) == 0)
		{
			// 바로 뒤의 토큰(i+1)이 문자열이면
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
			// 바로 뒤의 토큰 (i + 1)부터 배열의 요소
			// 인덱스를 지정한 토큰이 문자열이면서 배열이면
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
	// JSON 문서에 저장할 데이터
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

	// JSON 문법에 맞춰서 fprintf 함수로 값 출력
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
