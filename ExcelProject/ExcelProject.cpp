
#include <iostream>
#include <array>
#include <string>
#include <fstream>
#include <sstream>
#include "Stack.h"
#include "Cell.h"
#include "Table.h"
#include <typeinfo>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <cctype>
#include "MyString.h"

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

template <typename T>
void print_set(std::set<T>& st)
{
	std::cout << "[ ";
	for (typename std::set<T>::iterator itr = st.begin(); itr != st.end(); ++itr)
	{
		std::cout << *itr << " ";
	}
	std::cout << " ]" << std::endl;
}

template <typename K, typename V>
void print_map(std::map<K, V>& mp)
{
	//std::cout << "[ ";
	//for (typename std::map<K, V>::iterator iter = mp.begin(); iter != mp.end(); ++iter)
	/*{
		std::cout << iter->first << " " << iter->second << std::endl;
	}*/
	for (const auto& kv : mp)
	{
		std::cout << kv.first << " " << kv.second << std::endl;
	}
	
	std::cout << std::endl;
}

template <typename K, typename V>
void search_and_print(std::map<K, V>& m, K key)
{
	auto itr = m.find(key);
	if (itr != m.end())
	{
		std::cout << key << " --> " << itr->second << std::endl;
	}
	else
	{
		std::cout << key << " cannot found!" << std::endl;
	}
}

template <typename K>
void print_unordered_set(const std::unordered_set<K>& m)
{
	for (const auto& elem : m)
	{
		std::cout << elem << std::endl;
	}
}

template <typename K>
void isExist(std::unordered_set<K>& s, K key)
{
	auto itr = s.find(key);
	if (itr != s.end())
	{
		std::cout << key << " found!" << std::endl;
	}
	else
	{
		std::cout << key << " not found !" << std::endl;
	}
}

class Todo
{
	int priority;
	std::string job_desc;

public:
	Todo(int priority, std::string jobDesc) : priority(priority), job_desc(jobDesc) {}

	bool operator==(const Todo& t) const {
		if (priority == t.priority && job_desc == t.job_desc) 
			return true;
		else
			return false;
	}

	friend std::ostream& operator<<(std::ostream& o, const Todo& t);
	friend struct std::hash<Todo>;
};

namespace std
{
	template <>			// hash 클래스의 Todo 템플릿 특수화
	struct hash<Todo>
	{
		size_t operator()(const Todo& t) const
		{
			hash<string> hash_func;

			return t.priority ^ (hash_func(t.job_desc));
		}
	};
}

std::ostream& operator<<(std::ostream& o, const Todo& t)
{
	o << "[중요도 : " << t.priority << " ] " << t.job_desc;
	return o;
}

//struct int_compare
//{
//	bool operator()(const int& a, const int& b) const { return a > b; }
//};
template <typename T>
struct GreaterComp
{
	bool operator()(const T& a, const T& b) const { return a > b; }
};

template <typename Iter>
void print(Iter begin, Iter end)
{
	while (begin != end)
	{
		std::cout << *begin << " ";
		begin++;
	}

	std::cout << std::endl;
}

struct User
{
	std::string name;
	int level;

	User(std::string name, int level) : name(name), level(level) {}

	bool operator<(const User& u) const { return level < u.level; }
	bool operator==(const User& user) const
	{
		if (name == user.name && level == user.level) return true;
		return false;
	}
};

class Party
{
	std::vector<User> users;

public:
	bool addUser(std::string name, int level)
	{
		User newUser(name, level);
		if (std::find(users.begin(), users.end(), newUser) != users.end())
		{
			return false;
		}
		users.push_back(newUser);
		return true;
	}

	bool canJoinDungeon()
	{
		return std::all_of(users.begin(), users.end(), [](User& user) { return user.level >= 15; });
	}

	bool canUserSpecialItem()
	{
		return std::any_of(users.begin(), users.end(), [](User& user) {return user.level >= 19; });
	}
};

std::ostream& operator<<(std::ostream& o, const User& u)
{
	o << u.name << " , " << u.level;
	return o;
}

void TestString()
{
	std::cout << StrCat(string("this"), " ", "is", " ", string("a"), " ", string("sentence"));
	int a = 1, b = 2;
	std::cout << "Max (" << a << "," << b << ") ? : " << max(a, b) << std::endl;

	string s = "hello", t = "world";
	std::cout << "max (" << s << "," << t << ") ? : " << max(s, t) << std::endl;
}

void TestTypedefClass()
{
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
}

void TestList()
{
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

void TestSet()
{
	std::cout << "----------Set !----------" << std::endl;
	std::set<int> st;
	st.insert(10);
	st.insert(60);
	st.insert(20);
	st.insert(30);
	st.insert(40);
	st.insert(50);

	std::cout << "정렬" << std::endl;
	print_set(st);

	std::map<std::string, double> pitcher_list;

	pitcher_list.insert(std::pair<std::string, double>("박세웅", 2.23));
	pitcher_list.insert(std::pair<std::string, double>("해커", 2.93));
	pitcher_list.insert(std::pair<std::string, double>("피어밴드", 2.95));

	pitcher_list.insert(std::make_pair("차우준", 3.04));
	pitcher_list.insert(std::make_pair("장원준", 3.05));
	pitcher_list.insert(std::make_pair("헥터", 3.09));

	pitcher_list["니퍼트"] = 3.56;
	pitcher_list["박종훈"] = 3.76;
	pitcher_list["켈리"] = 3.90;

	print_map(pitcher_list);

	search_and_print(pitcher_list, std::string("박세웅"));
	search_and_print(pitcher_list, std::string("류현진"));
}

void TestVector()
{
	std::vector<int> vecInt;
	vecInt.push_back(10);
	vecInt.push_back(20);
	vecInt.push_back(30);
	vecInt.push_back(40);

	std::cout << "vec" << std::endl;
	print_vector(vecInt);

	std::vector<int>::iterator itr = vecInt.begin() + 2;

	*itr = 50;

	std::cout << "--------------" << std::endl;
	print_vector(vecInt);
	std::cout << ")))))))))))))))))" << std::endl;
	print_vector_range_based(vecInt);

	std::vector<int>::const_iterator citr = vecInt.cbegin() + 2;

	std::vector<int>::reverse_iterator r_iter = vecInt.rbegin();
	for (; r_iter != vecInt.rend(); ++r_iter)
	{
		std::cout << *r_iter << std::endl;
	}

	std::vector<User> vec;
	for (int i = 0; i < 100; i++)
	{
		std::string name = "";
		name.push_back('a' + i / 26);
		name.push_back('a' + i % 26);
		vec.push_back(User(name, static_cast<int>(rand() % 10)));
	}

	std::vector<User> vec2 = vec;

	std::cout << "before sort" << std::endl;
	print(vecInt.begin(), vecInt.end());

	std::sort(vecInt.begin(), vecInt.end());

	std::cout << "after sort" << std::endl;
	print(vecInt.begin(), vecInt.end());

	std::cout << "stable sort" << std::endl;
	std::stable_sort(vec2.begin(), vec2.end());
	print(vec2.begin(), vec2.end());

	print(vecInt.begin(), vecInt.end());
	std::cout << "----------------------------------------" << std::endl;
	std::sort(vecInt.begin(), vecInt.end(), GreaterComp<int>());		

	std::cout << "정렬 후" << std::endl;
	print(vecInt.begin(), vecInt.end());

	std::cout << "----------------------------------------" << std::endl;
	vecInt.clear();

	vecInt.push_back(5);
	vecInt.push_back(3);
	vecInt.push_back(2);
	vecInt.push_back(2);
	vecInt.push_back(3);
	vecInt.push_back(4);

	std::cout << "처음 vec 상태 ======" << std::endl;
	print(vecInt.begin(), vecInt.end());

	std::cout << "벡터 전체에 1을 더한다." << std::endl;
	std::transform(vecInt.begin(), vecInt.end(), vecInt.begin(), [](int i) {return i + 1; });
	print(vecInt.begin(), vecInt.end());

	auto result = std::find(vecInt.begin(), vecInt.end(), 3);
	std::cout << "3은 " << std::distance(vecInt.begin(), result) + 1 << "번째 원소" << std::endl;

	auto current = vecInt.begin();
	while (true)
	{
		current = std::find(current, vecInt.end(), 3);
		if (current == vecInt.end()) break;
		std::cout << "3은 " << std::distance(vecInt.begin(), current) + 1 << "번 째 원소" << std::endl;
		current++;
	}

	current = vecInt.begin();
	while (true)
	{
		current = std::find_if(current, vecInt.end(), [](int i) { return i % 3 == 2; });
		if (current == vecInt.end()) break;
		std::cout << "3으로 나눈 나머지가 2인 원소는 " << *current << std::endl;
		current++;
	}
}

void TestUnorderedSet()
{
	std::cout << "------------------------" << std::endl;
	std::unordered_set<std::string> uos;

	uos.insert("hi");
	uos.insert("my");
	uos.insert("name");
	uos.insert("is");
	uos.insert("psi");

	print_unordered_set(uos);
	isExist(uos, std::string("hi"));

	uos.erase(std::string("hi"));			// uos.erase(uos.find("hi"));
	isExist(uos, std::string("hi"));
}

void TestParty()
{
	Party party;
	party.addUser("전사", 15);
	party.addUser("도적", 18);
	party.addUser("주술", 12);
	party.addUser("도사", 19);

	std::cout << std::boolalpha;
	std::cout << "던전 입장 가능 ? " << party.canJoinDungeon() << std::endl;
	std::cout << "특별 아이템 사용가능 ? " << party.canUserSpecialItem() << std::endl;
}

// char_traits의 모든 함수들은 static 함수이다.
struct myCharTraits : public std::char_traits<char>
{
	static int getRealRank(char c)
	{
		// 숫자면 순위를 엄청 떨어트린다.
		if (isdigit(c))
			return c + 256;

		return c;
	}

	static bool lt(char c1, char c2)
	{
		return getRealRank(c1) < getRealRank(c2);
	}

	static int compare(const char* s1, const char* s2, size_t n)
	{
		while (n-- != 0)
		{
			if (getRealRank(*s1) < getRealRank(*s2)) { return -1; }
			if (getRealRank(*s1) > getRealRank(*s2)) { return 1; }
			++s1;
			++s2;
		}

		return 0;
	}
};

void TestBasicString()
{
	std::basic_string<char, myCharTraits> my_s1 = "1a";
	std::basic_string<char, myCharTraits> my_s2 = "a1";


	std::cout << "우선 순위 : " << std::boolalpha << (my_s1 < my_s2) << std::endl;

	std::string s1 = "1a";
	std::string s2 = "a1";

	std::cout << "우선 순위 : " << std::boolalpha << (s1 < s2) << std::endl;
}

void* operator new(std::size_t count)
{
	std::cout << count << " bytes 할당" << std::endl;
	return malloc(count);
}

void SSOTest()
{
	std::cout << "s1 constructor" << std::endl;
	std::string s1 = "this is a pretty long sentence !!";
	std::cout << "s1 size : " << sizeof(s1) << std::endl;			// 28

	std::cout << "s2 constructor" << std::endl;
	std::string s2 = "short sentence";
	std::cout << "s2 size : " << sizeof(s2) << std::endl;			// 28
}

void TestUTF()
{
	//std::string str = "hello";		// char[]
	//std::wstring wstr = L"hello";	// wchar_t[]
	std::u32string u32_str = U"이건 UTF-32 문자열입니다.";
	std::cout << u32_str.size() << std::endl;			// 17

	std::string str8 = u8"이건 UTF-8 문자열 입니다.";
	std::cout << str8.size() << std::endl;				// 33
	
	size_t i = 0;
	size_t len = 0;

	while (i < str8.size())
	{
		int char_size = 0;
		if ((str8[i] & 0b11111000) == 0b11110000)
			char_size = 4;
		else if ((str8[i] & 0b11110000) == 0b11100000)
			char_size = 3;
		else if ((str8[i] & 0b11100000) == 0b11000000)
			char_size = 2;
		else if ((str8[i] & 0b10000000) == 0b00000000)
			char_size = 1;
		else
		{
			std::cout << "이상한 문자" << std::endl;
			char_size = 1;
		}

		std::cout << str8.substr(i, char_size) << std::endl;

		i += char_size;
		len++;
	}

	std::cout << "문자열의 실제 길이 : " << len << std::endl;

	std::u16string u16_str = u"이건 UTF-16 문자열 입니다.";
	std::cout << u16_str.size() << std::endl;
}

void MyStringTest()
{
	MyString str1("very very very long string");
	MyString str2("<some string inserted between>");
	str1.Reserve(30);

	std::cout << "Capacity : " << str1.Capacity() << std::endl;
	std::cout << "string length : " << str1.Length() << std::endl;
	str1.Insert(5, str2);
	str1.Println();
}

class CopyElisionT
{
	int _data;

public:
	CopyElisionT(int data) : _data(data) { std::cout << "일반 생성자 호출!" << std::endl; }

	CopyElisionT(const CopyElisionT& t) : _data(t._data) { std::cout << "복사 생성자 호출!" << std::endl; }
};

void CopyElisionTest()
{
	CopyElisionT cp(1);
	CopyElisionT cp1(cp);

	CopyElisionT cp2(CopyElisionT(2));
}

template <typename T>
void mySwap(T &a, T &b)
{
	MyString str1("abc");
	MyString str2("def");
	std::cout << "before swap--" << std::endl;
	std::cout << "str1 : ";
	str1.Println();
	std::cout << "str2 : ";
	str2.Println();

	std::cout << "swap -- " << std::endl;
}

int main()
{
	
}
