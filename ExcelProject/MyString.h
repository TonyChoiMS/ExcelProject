#pragma once
class MyString
{
private:
	char* stringContent;			// ���ڿ� �����͸� ����Ű�� ������
	int stringLength;				// ���ڿ� ����
	int memoryCapacity;				// �޸� ������ ũ��

public:
	explicit MyString(int capacity);			// �Ҵ��� �޸� ������ �̸� ����.
	MyString(char c);				// ���ڿ� �ϳ��� ����
	MyString(const char* str);		// ���ڿ��κ��� ����
	MyString(const MyString& str);	// ���������
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

