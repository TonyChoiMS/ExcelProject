#pragma once

#include <string>


template <typename T>
class Vector
{
private:
	T* data;
	int capacity;
	int length;

public:
	typedef T value_type;		// � Ÿ���� �����ϴ���
	Vector(int n = 1) : data(new T[n]), capacity(n), length(0) {}
	~Vector()
	{
		if (data)
			delete[] data;
	}


	void pushBack(T s)
	{
		if (capacity <= length)
		{
			T* temp = new T[capacity * 2];
			for (int i = 0; i < length; i++)
			{
				temp[i] = data[i];
			}
			delete[] data;
			data = temp;
			capacity *= 2;
		}

		data[length] = s;
		length++;
	}

	T operator[](int i) { return data[i]; }

	void remove(int x)
	{
		for (int i = x + 1; i < length; i++)
		{
			data[i - 1] = data[i];
		}
		length--;
	}

	int size() { return length; }

	// i��° ���ҿ� j��° ���� ��ġ�� �ٲ۴�.
	void swap(int i, int j)
	{
		T temp = data[i];
		data[i] = data[j];
		data[j] = temp;
	}
};


class Table;

using std::string;

class Cell
{
protected:
	int x, y;
	Table* table;

public:
	Cell(int x, int y, Table* table);
	~Cell() {}

	virtual string stringify() = 0;
	virtual int toNumeric() = 0;
};

class StringCell : public Cell
{
	string data;

public:
	string stringify() override;
	int toNumeric() override;

	StringCell(string data, int x, int y, Table* t);
};

class NumberCell : public Cell
{
	int data;

public:
	string stringify() override;
	int toNumeric() override;

	NumberCell(int data, int x, int y, Table* t);
};

class DateCell : public Cell
{
	time_t data;

public:
	string stringify() override;
	int toNumeric() override;

	DateCell(string s, int x, int y, Table* t);
};

class ExprCell : public Cell
{
	string data;
	string* parsed_expr;

	Vector<string> exp_vec;

	// ������ �켱 ������ ��ȯ�մϴ�.
	int precedence(char c);

	// ������ �м��մϴ�.
	void parse_expression();

public:
	ExprCell(string data, int x, int y, Table* t);

	string stringify() override;
	int toNumeric() override;
};
