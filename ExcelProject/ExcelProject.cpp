
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
#include <vector>
#include <list>

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

template <typename a, typename b>
struct divide
{
	typedef Int<a::num / b::num> result;
};

using one = Int<1>;
using two = Int<2>;
using three = Int<3>;

template <typename a, typename b>
struct Divide
{
	typedef Int<a::num / b::num> result;
};

template <typename N, typename d>
struct check_div
{
	// result 중에 한 개라도 true면 전체가 true
	static const bool result = (N::num % d::num == 0) || check_div<N, typename Add<d, one>::result>::result;
};

template <typename N>
struct _is_prime
{
	static const bool reuslt = !check_div<N, two>::result;
};

template <>
struct _is_prime<two>
{
	static const bool result = true;
};

template<>
struct _is_prime<three>
{
	static const bool result = true;
};

template <typename N>
struct check_div<N, typename Divide<N, two>::result>
{
	static const bool result = (N::num % (N::num / 2) == 0);
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

// 최대 공약수 (유클리드 호제법)
int gcd(int a, int b)
{
	if (b == 0)
		return a;

	return gcd(b, a % b);
}

// 템플릿을 사용한 최대 공약수
template <int X, int Y>
struct GCD
{
	static const int value = GCD<Y, X % Y>::value;
};

template <int X>
struct GCD<X, 0>
{
	static const int value = X;
};

template <int N, int D = 1>
struct Ratio
{
private:
	const static int _gcd = GCD<N, D>::value;

public:
	typedef Ratio<N / _gcd, D / _gcd> type;
	static const int num = N / _gcd;
	static const int den = D / _gcd;
};

template <class R1, class R2>
struct _Ratio_Add
{
	typedef Ratio<R1::num * R2::den + R2::num * R1::den, R1::den * R2::den> type;
};

template <class R1, class R2>
struct Ratio_Add : _Ratio_Add <R1, R2>::type{};

template <class R1, class R2>
struct _Ratio_subtract
{
	using type = Ratio<R1::num * R2::den - R2::num * R1::den, R1::den * R2::den>;
};

template <class R1, class R2>
struct Ratio_subtract : _Ratio_subtract<R1, R2>::type {};

template <class R1, class R2>
struct _Ratio_multiply
{
	using type = Ratio<R1::num * R2::num, R1::den * R2::den>;
};

template <class R1, class R2>
struct Ratio_muiltiply : _Ratio_multiply<R1, R2>::type {};

template <class R1, class R2>
struct _Ratio_divide
{
	using type = Ratio<R1::num * R2::den, R1::den * R2::num>;
};

template <class R1, class R2>
struct Ratio_divide : _Ratio_divide<R1, R2>::type {};

template <typename U, typename V, typename W>
struct Dim
{
	using M = U;
	using L = V;
	using T = W;

	using type = Dim<M, L, T>;
};

template <typename U, typename V>
struct add_dim_
{
	typedef Dim<typename Ratio_Add<typename U::M, typename V::M>::type,
		typename Ratio_Add<typename U::L, typename U::L>::type,
		typename Ratio_Add<typename U::T, typename V::T>::type>
		type;
};

template <typename U, typename V>
struct subtract_dim_
{
	typedef Dim<typename Ratio_subtract<typename U::M, typename V::M>::type,
		typename Ratio_subtract<typename U::L, typename U::L>::type,
		typename Ratio_subtract<typename U::T, typename V::T>::type>
		type;
};

template <typename T, typename D>
struct quantity
{
	T q;
	using dim_type = D;

	quantity operator+(quantity<T, D> quant) { return quantity<T, D>(q + quant.q); }
	quantity operator-(quantity<T, D> quant) { return quantity<T, D>(q + quant.q); }

	template <typename D2>
	quantity<T, typename add_dim_<D, D2>::type> operator*(quantity<T, D2> quant)
	{
		return quantity<T, typename add_dim_<D, D2>::type>(q * quant.q);
	}

	template <typename D2>
	quantity<T, typename subtract_dim_<D, D2>::type> operator/(
		quantity<T, D2> quant) {
		return quantity<T, typename subtract_dim_<D, D2>::type>(q / quant.q);
	}

	quantity<T, D> operator*(T scalar) { return quantity<T, D>(q * scalar); }
	quantity<T, D> operator/(T scalar) { return quantity<T, D>(q / scalar); }

	quantity(T q) : q(q) {}
};

template <typename T, typename D>
std::ostream& operator<<(std::ostream& out, const quantity<T, D>& q)
{
	out << q.q << "kg^^" << D::M::num / D::M::den << "m^" << D::L::num / D::L::den << "s^" << D::T::num / D::T::den;
	return out;
}

template <typename T>
void print_vector(std::vector<T>& vec)
{
	for (typename std::vector<T>::iterator itr = vec.begin(); itr != vec.end(); ++itr)
	{
		std::cout << *itr << std::endl;
	}
}

template <typename T>
void print_vector_range_based(std::vector<T>& vec)
{
	for (const auto& elem : vec)
	{
		std::cout << elem << std::endl;
	}
}

template <typename T>
void print_list(std::list<T>& lst)
{
	std::cout << "[ ";
	for (const auto& elem : lst)
	{
		std::cout << elem << " ";
	}

	std::cout << "]" << std::endl;
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

	std::cout << "gcd(36, 24) :: " << GCD<36, 24>::value << ", fucntion : " << gcd(36, 24) << std::endl;

	typedef Ratio<2, 3> rat;
	typedef Ratio<3, 2> rat2;
	//typedef Ratio_Add<rat, rat2> rat3;			// 아래 구문과 같은 의미.
	using rat3 = Ratio_Add<rat, rat2>;
	
	std::cout << rat3::num << " / " << rat3::den << std::endl;

	typedef void(*func)(int, int);
	using func = void(*)(int, int);

	std::cout << "----------------------------------" << std::endl;

	std::vector<int> vec;
	vec.push_back(10);
	vec.push_back(20);
	vec.push_back(30);
	vec.push_back(40);

	std::cout << "vec" << std::endl;
	print_vector(vec);

	std::vector<int>::iterator itr = vec.begin() + 2;

	*itr = 50;

	std::cout << "--------------" << std::endl;
	print_vector(vec);
	std::cout << ")))))))))))))))))" << std::endl;
	print_vector_range_based(vec);

	std::vector<int>::const_iterator citr = vec.cbegin() + 2;

	std::vector<int>::reverse_iterator r_iter = vec.rbegin();
	for (; r_iter != vec.rend(); ++r_iter)
	{
		std::cout << *r_iter << std::endl;
	}

	std::list<int> lst;
	lst.push_back(10);
	lst.push_back(20);
	lst.push_back(30);
	lst.push_back(40);

	print_list(lst);
	std::cout << "--------------------" << std::endl;
	for (std::list<int>::iterator itr = lst.begin(); itr != lst.end(); ++itr)
	{
		if (*itr == 20)
			lst.insert(itr, 50);
	}
	std::cout << "--------------------" << std::endl;
	print_list(lst);

	for (std::list<int>::iterator itr = lst.begin(); itr != lst.end(); ++itr)
	{
		if (*itr == 30)
		{
			lst.erase(itr);
			break;
		}
			
	}
	std::cout << "--------------------" << std::endl;
	print_list(lst);
}
