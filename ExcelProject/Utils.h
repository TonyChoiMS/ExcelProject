#pragma once

#ifndef UTILS_H
#define UTILS_H


#include <string>
using std::string;

namespace MyExcel
{
	class Vector
	{
		string* data;
		int capacity;
		int length;

	public:
		Vector(int n = 1);
		~Vector();

		void pushBack(string s);
		string operator[](int i);
		void remove(int x);
		int size();
	};

	class Stack
	{
		struct Node
		{
			Node* prev;
			string s;

			Node(Node* prev, string s) : prev(prev), s(s) {}
		};

		Node* current;
		Node start;

	public:
		Stack();
		~Stack();

		void push(string s);
		string pop();
		string peek();
		bool isEmpty();
	};

	class NumStack
	{
		struct Node
		{
			Node* prev;
			double s;

			Node(Node* prev, double s) : prev(prev), s(s) {}
		};

		Node* current;
		Node start;

	public:
		NumStack();
		~NumStack();

		void push(double s);
		double pop();
		double peek();
		bool isEmpty();
	};
}

#endif