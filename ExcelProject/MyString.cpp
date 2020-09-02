#include "MyString.h"
#include <iostream>
#include <algorithm>

MyString::MyString(int capacity)
{
	stringContent = new char[capacity];
	stringLength = 0;
	memoryCapacity = capacity;
}

MyString::MyString(char c)
{
	stringContent = new char[1];
	stringContent[0] = c;
	stringLength = 1;
	memoryCapacity = 1;
}

MyString::MyString(const char * str)
{
	stringLength = strlen(str);
	memoryCapacity = stringLength;
	stringContent = new char[stringLength];

	for (int i = 0; i != stringLength; i++)
	{
		stringContent[i] = str[i];
	}
}

MyString::MyString(const MyString & str)
{
	stringLength = str.stringLength;
	memoryCapacity = str.stringLength;
	stringContent = new char[stringLength];
	for (int i = 0; i != stringLength; i++)
	{
		stringContent[i] = str.stringContent[i];
	}
}

MyString::~MyString()
{
	delete[] stringContent;
}

int MyString::Length() const
{
	return stringLength;
}

void MyString::Print()
{
	for (int i = 0; i != stringLength; i++)
	{
		std::cout << stringContent[i];
	}
}

void MyString::Println()
{
	for (int i = 0; i != stringLength; i++)
	{
		std::cout << stringContent[i];
	}

	std::cout << std::endl;
}

bool MyString::operator==(MyString & str)
{
	return !Compare(str);
}

MyString & MyString::assign(const MyString & str)
{
	if (str.stringLength > memoryCapacity)
	{
		delete[] stringContent;

		stringContent = new char[str.stringLength];
		memoryCapacity = str.stringLength;
	}

	for (int i = 0; i != str.stringLength; i++)
	{
		stringContent[i] = str.stringContent[i];
	}

	stringLength = str.stringLength;

	return *this;
}

MyString & MyString::assign(const char * str)
{
	int strLength = strlen(str);
	if (strLength > memoryCapacity)
	{
		delete[] stringContent;

		stringContent = new char[strLength];
		memoryCapacity = strLength;
	}

	for (int i = 0; i != strLength; i++)
	{
		stringContent[i] = str[i];
	}

	stringLength = strLength;

	return *this;
}

void MyString::Reserve(int size)
{
	if (size > memoryCapacity)
	{
		char *prevStringContent = stringContent;

		stringContent = new char[size];
		memoryCapacity = size;

		for (int i = 0; i != stringLength; i++)
		{
			stringContent[i] = prevStringContent[i];
		}

		delete[] prevStringContent;
	}
}

char MyString::at(int i)
{
	if (i >= stringLength || i < 0)
		return NULL;
	else
		return stringContent[i];
}

MyString & MyString::Insert(int loc, const char * str)
{
	MyString temp(str);
	return Insert(loc, temp);
}

MyString & MyString::Insert(int loc, char c)
{
	MyString temp(c);
	return Insert(loc, temp);
}

MyString & MyString::Insert(int loc, const MyString & str)
{
	// 이는 i의 위치 바로 앞에 문자를 삽입하게 됨.
	if (loc < 0 || loc > stringLength) return *this;

	if (stringLength + str.stringLength > memoryCapacity)
	{
		if (memoryCapacity * 2 > stringLength + str.stringLength)
			memoryCapacity *= 2;
		else
			memoryCapacity = stringLength + str.stringLength;

		char* prevStringContent = stringContent;
		stringContent = new char[memoryCapacity];

		int i;
		for (i = 0; i < loc; i++)
		{
			stringContent[i] = prevStringContent[i];
		}

		for (int j = 0; j != str.stringLength; j++)
		{
			stringContent[i + j] = str.stringContent[j];
		}

		for (; i < stringLength; i++)
		{
			stringContent[str.stringLength + i] = prevStringContent[i];
		}

		delete[] prevStringContent;

		stringLength = stringLength + str.stringLength;
		return *this;
	}

	for (int i = stringLength - 1; i >= loc; i--)
	{
		stringContent[i + str.stringLength] = stringContent[i];
	}

	for (int i = 0; i < str.stringLength; i++)
	{
		stringContent[i + loc] = str.stringContent[i];
	}

	stringLength = stringLength + str.stringLength;
	return *this;
}

MyString & MyString::Erase(int loc, int num)
{
	if (num < 0 || loc < 0 || loc > stringLength) return *this;

	if (num > stringLength)
		num = stringLength;

	for (int i = loc + num; i < stringLength; i++)
	{
		stringContent[i - num] = stringContent[i];
	}

	stringLength -= num;
	return *this;
}

int MyString::Find(int findFrom, MyString & str) const
{
	int i, j;
	if (str.stringLength == 0) return -1;
	for (i = findFrom; i <= stringLength - str.stringLength; i++)
	{
		for (j = 0; j < str.stringLength; j++)
		{
			if (stringContent[i + j] != str.stringContent[j]) break;
		}

		if (j == str.stringLength) return i;
	}

	// 못찾음
	return -1;
}

int MyString::Find(int findFrom, const char * str) const
{
	MyString temp(str);
	return Find(findFrom, temp);
}

int MyString::Find(int findFrom, char c) const
{
	MyString temp(c);
	return Find(findFrom, temp);
}

int MyString::Compare(const MyString & str) const
{
	for (int i = 0; i < std::min(stringLength, str.stringLength); i++)
	{
		if (stringContent[i] > str.stringContent[i])
			return 1;
		else if (stringContent[i] < str.stringContent[i])
			return -1;
	}

	if (stringLength == str.stringLength) return 0;
	else if (stringLength > str.stringLength) return 1;
	else return -1;
}
