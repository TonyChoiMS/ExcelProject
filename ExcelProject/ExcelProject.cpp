
#include <iostream>
#include <array>
#include <string>
#include <fstream>
#include <sstream>
#include "Vector.h"
#include "Stack.h"
#include "Cell.h"
#include "Table.h"
#include <typeinfo>

#ifndef UTILS_H
#define UTILS_H

using std::string;

namespace MyExcel
{
	// Class Excel
	class Excel
	{
		Table* current_table;

	public:
		Excel(int maxRow, int maxCol, int choice);

		int parseUserInput(string s);
		void commandLine();
	};

	Excel::Excel(int maxRow, int maxCol, int choice = 0)
	{
		switch (choice) 
		{
			case 0:
				current_table = new TxtTable(maxRow, maxCol);
			break;
			case 1:
				current_table = new CSVTable(maxRow, maxCol);
			break;
			default:
				current_table = new HtmlTable(maxRow, maxCol);
		}
	}

	int Excel::parseUserInput(string s)
	{
		int next = 0;
		string command = "";
		for (size_t i = 0; i < s.length(); i++)
		{
			if (s[i] == ' ')
			{
				command = s.substr(0, i);
				next = i + 1;
				break;
			}
			else if (i == s.length() - 1)
			{
				command = s.substr(0, i + 1);
				next = i + 1;
				break;
			}
		}

		string to = "";
		for (size_t i = next; i < s.length(); i++)
		{
			if (s[i] = ' ' || i == s.length() - 1)
			{
				to = s.substr(next, i - next);
				next = i + 1;
				break;
			}
			else if (i == s.length() - 1)
			{
				to = s.substr(0, i + 1);
				next = i + 1;
				break;
			}
		}

		// Cell 이름으로 받는다.
		int col = to[0] - 'A';
		int row = atoi(to.c_str() + 1) - 1;

		string rest = s.substr(next);

		if (command == "sets")
		{
			current_table->RegCell(new StringCell(rest, row, col, current_table), row, col);
		}
		else if (command == "setn")
		{
			current_table->RegCell(new NumberCell(atoi(rest.c_str()), row, col, current_table), row, col);
		}
		else if (command == "setd")
		{
			current_table->RegCell(new DateCell(rest, row, col, current_table), row, col);
		}
		else if (command == "out")
		{
			std::ofstream out(to);
			//out << *current_table;
			std::cout << to << "에 내용이 저장되었습니다." << std::endl;
		}
		else if (command == "exit")
		{
			return 0;
		}

		return 1;
	}

	void Excel::commandLine()
	{
		string s;
		//std::getline(cin, s);

		while (parseUserInput(s))
		{
			//std::cout << *current_table << std::endl << ">> ";
			//std::getline(cin, s);
		}
	}
}

#endif

template <typename T>
T max(T& a, T& b)
{
	return a > b ? a : b;
}

template <typename Cont>
void BubbleSort(Cont& cont)
{
	for (int i = 0; i < cont.size(); i++)
	{
		for (int j = i + 1; j< cont.size(); j++)
		{
			if (cont[i] > cont[j])
				cont.swap(i, j);
		}
	}
}

template <typename Cont, typename Comp>
void BubbleSort(Cont& cont, Comp& comp)
{
	for (int i = 0; i < cont.size(); i++)
	{
		for (int j = i + 1; j < cont.size(); j++)
		{
			if (!comp(cont[i], cont[j]))
				cont.swap(i, j);
		}
	}
}

struct Comp1 { bool operator()(int a, int b) { return a > b; } };

struct Comp2 { bool operator()(int a, int b) { return a < b; } };


template <typename T>
void print_array(const T& arr)
{
	for (int i = 0; i < arr.size(); i++)
	{
		std::cout << arr[i] << " ";
	}
	std::cout << std::endl;
}

template <typename String>
string StrCat(const String& s)
{
	return string(s);
}

size_t GetStringSize(const char* s) { return strlen(s); }

size_t GetStringSize(const string& s) { return s.size(); }

template <typename String, typename... Strings>
size_t GetStringSize(const String& s, Strings... strs)
{
	return GetStringSize(s) + GetStringSize(strs...);
}

void AppendToString(string* concat_str) { return; }

template <typename String, typename... Strings>
void AppendToString(string* concat_str, const String& s, Strings... strs)
{
	concat_str->append(s);
	AppendToString(concat_str, strs...);
}

template <typename String, typename... Strings>
string StrCat(const String& s, Strings... strs)
{
	// 먼저 합쳐질 문자열의 총 길이를 구한다.
	size_t total_size = GetStringSize(s, strs...);

	// reserve를 통해 미리 공간을 할당해놓는다.
	string concat_str;
	concat_str.reserve(total_size);

	concat_str = s;

	// concat_str에 문자열들을 붙인다.
	AppendToString(&concat_str, strs...);

	return concat_str;
}

// 템플릿 타입을 객체처럼 사용
template <int N>
struct Int
{
	static const int num = N;
};

template <typename T, typename U>
struct Add
{
	typedef Int<T::num + U::num> result;
};

// Factorial 템플릿으로 구현
template <int N>
struct Factorial
{
	static const int result = N * Factorial<N - 1>::result;
};

template<>
struct Factorial<1>
{
	static const int result = 1;
};

// Factorial 일반 함수 구현
int factorial(int n)
{
	if (n == 1) return 1;

	return n * factorial(n - 1);
}


int main()
{
	std::cout << StrCat(string("this"), " ", "is", " ", string("a"), " ", string("sentence"));
	int a = 1, b = 2;
	std::cout << "Max (" << a << "," << b << ") ? : " << max(a, b) << std::endl;

	string s = "hello", t = "world";
	std::cout << "max (" << s << "," << t << ") ? : " << max(s, t) << std::endl;

	typedef Int<1> one;
	typedef Int<2> two;

	typedef Add<one, two>::result three;

	std::cout << "additional result : " << three::num << std::endl;

	std::cout << " 6! = 1 * 2 * 3 * 4 * 5 * 6 = " << Factorial<6>::result << std::endl;
}
