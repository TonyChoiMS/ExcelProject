#include "Array.h"


template <typename T, unsigned int N>
Array<T, N>::Array(T (&arr)[N])
{
	for (int i = 0; i < N; i++)
	{
		data[i] = arr[i];
	}
}

template <typename T, unsigned int N>
Array<T, N>::~Array()
{
}

template<typename T, unsigned int N>
void Array<T, N>::PrintAll()
{
	for (int i = 0; i < N; i++)
	{
		std::cout << data[i] << ", ";
	}

	std::cout << std::endl;
}
