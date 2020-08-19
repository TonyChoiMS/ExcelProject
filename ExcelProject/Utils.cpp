#include "Utils.h"

namespace MyExcel
{
	Vector::Vector(int n) : data(new string[n]), capacity(n), length(0) {}
	
	Vector::~Vector()
	{
		if (data)
			delete[] data;
	}

	void Vector::pushBack(string s)
	{
		if (capacity <= length)
		{
			string* temp = new string[capacity * 2];
			for (int i = 0; i < length ; i++)
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

	string Vector::operator[](int i)
	{
		return data[i];
	}

	void Vector::remove(int x)
	{
		for (int i = x + 1; i < length; i++)
		{
			data[i - 1] = data[i];
		}
		length--;
	}

	int Vector::size()
	{
		return length;
	}

	Stack::Stack() : start(NULL, "") { current = &start; }

	Stack::~Stack()
	{
		while (current != &start)
		{
			Node* prev = current;
			current = current->prev;
			delete prev;
		}
	}

	void Stack::push(string s)
	{
		Node* n = new Node(current, s);
		current = n;
	}

	string Stack::pop()
	{
		if (current == &start) return "";

		string s = current->s;
		Node* prev = current;
		current = current->prev;

		delete prev;
		return s;
	}

	string Stack::peek()
	{
		return current->s;
	}

	bool Stack::isEmpty()
	{
		if (current == &start) return true;
		else return false;
	}

	NumStack::NumStack() : start(NULL, 0) { current = &start; }

	NumStack::~NumStack()
	{
		while (current != &start)
		{
			Node* prev = current;
			current = current->prev;
			delete prev;
		}
	}

	void NumStack::push(double s)
	{
		Node* n = new Node(current, s);
		current = n;
	}

	double NumStack::pop()
	{
		if (current == &start) return 0;

		double s = current->s;
		Node* prev = current;
		current = current->prev;

		delete prev;
		return s;
	}

	double NumStack::peek()
	{
		return current->s;
	}

	bool NumStack::isEmpty()
	{
		if (current == &start) return true;
	}
}