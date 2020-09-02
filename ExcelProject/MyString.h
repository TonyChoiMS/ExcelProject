#pragma once
class MyString
{
private:
	char* stringContent;			// 문자열 데이터를 가리키는 포인터
	int stringLength;				// 문자열 길이
	int memoryCapacity;				// 메모리 공간의 크기

public:
	explicit MyString(int capacity);			// 할당할 메모리 공간을 미리 지정.
	MyString(char c);				// 문자열 하나로 생성
	MyString(const char* str);		// 문자열로부터 생성
	MyString(const MyString& str);	// 복사생성자
	~MyString();

	int Length() const;
	void Print();
	void Println();

	bool operator==(MyString& str);

	MyString& assign(const MyString& str);
	MyString& assign(const char* str);

	int Capacity() { return memoryCapacity; }
	void Reserve(int size);
	char at(int i);
	MyString& Insert(int loc, const MyString& str);
	MyString& Insert(int loc, const char* str);
	MyString& Insert(int loc, char c);

	MyString& Erase(int loc, int num);

	int Find(int findFrom, MyString& str) const;
	int Find(int findFrom, const char* str) const;
	int Find(int findFrom, char c) const;

	int Compare(const MyString& str) const;
};

