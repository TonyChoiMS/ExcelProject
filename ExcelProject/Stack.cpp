#include "Stack.h"

using std::string;

// Stack~~~~
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

// NumStack~~~~
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