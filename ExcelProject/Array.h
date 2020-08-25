#pragma once

#include <iostream>

template <typename T, unsigned int N>
class Array
{
	T data[N];

public:
	Array(T (&arr)[N]);
	~Array();

	T* getArray() { return N; }

	void PrintAll();
};

