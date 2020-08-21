
#include <iostream>
#include <string>
#include <fstream>

#ifndef UTILS_H
#define UTILS_H

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

	class Table;

	class Cell
	{
	protected:
		int x, y;
		Table* table;

		string data;

	public:
		Cell(string data, int x, int y, Table* table);
		~Cell();

		virtual string stringify();
		virtual int toNumeric();


	};

	class Table
	{
	protected:
		int maxRowSize, maxColSize;		// 행 및 열의 최대 크기

		// 데이터를 보관하는 테이블
		// Cell*을 보관하는 2차원 배열
		Cell*** dataTable;

	public:
		Table(int maxRowSize, int maxColSize);
		~Table();

		// 새로운 셀을 row 행 col 열에 등록한다.
		void RegCell(Cell* c, int row, int col);

		// 해당 셀의 정수값을 반환한다.
		// s : 셀 이름(Ex. A3, B6과 같이)
		int toNumeric(const string& s);

		// 행 및 열 번호로 셀을 호출한다.
		int toNumeric(int row, int col);

		// 해당 셀의 문자열을 반환한다.
		string stringify(const string& s);
		string stringify(int row, int col);

		virtual string PrintTable() = 0;
	};

	class TxtTable : public Table
	{
		string repeatChar(int n, char c);

		// 숫자로 된 열 번호를 A, B, .... Z, AA, AB, ... 이런 순서로 매겨준다.
		string colNumToStr(int n);

	public:
		TxtTable(int row, int col);

		// 텍스트로 표를 깨끗하게 출력해준다.
		string PrintTable() override;
	};

	class HtmlTable : public Table
	{
	public:
		HtmlTable(int row, int col);

		string PrintTable() override;
	};

	class CSVTable : public Table
	{
	public:
		CSVTable(int row, int col);
		string PrintTable() override;
	};

	// Vector~~~~
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

	// Cell~~~~
	Cell::Cell(string data, int x, int y, Table* table) : data(data), x(x), y(y), table(table) {}

	string Cell::stringify() { return data; }

	int Cell::toNumeric() { return 0; }

	// Table~~~~
	Table::Table(int maxRowSize, int maxColSize) : maxRowSize(maxRowSize), maxColSize(maxColSize)
	{
		dataTable = new Cell**[maxRowSize];
		for (int i = 0; i < maxRowSize; i++)
		{
			dataTable[i] = new Cell*[maxRowSize];
			for (int j = 0; j < maxColSize; j++)
			{
				dataTable[i][j] = NULL;
			}
		}
	}

	Table::~Table()
	{
		for (int i = 0; i < maxRowSize; i++)
		{
			for (int j = 0; j < maxColSize; j++)
			{
				if (dataTable[i][j]) delete dataTable[i][j];
			}
		}

		for (int i = 0; i < maxRowSize; i++)
		{
			delete[] dataTable[i];
		}

		delete[] dataTable;
	}

	void Table::RegCell(Cell * c, int row, int col)
	{
		if (!(row < maxRowSize && col < maxColSize)) return;

		if (dataTable[row][col])
			delete dataTable[row][col];

		dataTable[row][col] = c;
	}

	int Table::toNumeric(const string & s)
	{
		int row = s[0] - 'A';
		int col = atoi(s.c_str() + 1) - 1;

		if (row < maxRowSize && col < maxColSize)
		{
			if (dataTable[row][col])
				return dataTable[row][col]->toNumeric();
		}

		return 0;
	}

	int Table::toNumeric(int row, int col)
	{
		if (row < maxRowSize && col < maxColSize && dataTable[row][col])
			return dataTable[row][col]->toNumeric();
		else
			return 0;
	}

	string Table::stringify(const string & s)
	{
		// Cell 이름으로 받는다.
		int col = s[0] - 'A';
		int row = atoi(s.c_str() + 1) - 1;

		if (row < maxRowSize && col < maxColSize)
		{
			if (dataTable[row][col])
				return dataTable[row][col]->stringify();
		}

		return 0;
	}

	string Table::stringify(int row, int col)
	{
		if (row < maxRowSize && col < maxColSize && dataTable[row][col])
			return dataTable[row][col]->stringify();

		return "";
	}


	std::ostream& operator<<(std::ostream& o, Table& table)
	{
		o << table.PrintTable();
		return o;
	}


	// TxtTable~~~~
	string TxtTable::repeatChar(int n, char c)
	{
		string s = "";
		for (int i = 0; i < n; i++)
		{
			s.push_back(c);
		}

		return s;
	}

	string TxtTable::colNumToStr(int n)
	{
		string s = "";
		if (n < 26)
		{
			s.push_back('A' + n);
		}
		else
		{
			char first = 'A' + n / 26 - 1;
			char second = 'A' + n % 26;

			s.push_back(first);
			s.push_back(second);
		}

		return s;
	}

	TxtTable::TxtTable(int row, int col) : Table(row, col) { }

	// 텍스트로 표를 깨끗하게 출력한다.
	string TxtTable::PrintTable()
	{
		string totalTable;

		int* colMaxWide = new int[maxColSize];
		for (int i = 0; i < maxColSize; i++)
		{
			unsigned int maxWide = 2;
			for (int j = 0; j < maxRowSize; j++)
			{
				if (dataTable[j][i] &&
					dataTable[j][i]->stringify().length() > maxWide)
				{
					maxWide = dataTable[j][i]->stringify().length();
				}
			}
			colMaxWide[i] = maxWide;
		}

		// 맨 상단에 열 정보 표시
		totalTable += "    ";
		int totalWide = 4;
		for (int i = 0; i < maxColSize; i++)
		{
			if (colMaxWide[i])
			{
				int maxLen = std::fmaxl(2, colMaxWide[i]);
				totalTable += " | " + colNumToStr(i);
				totalTable += repeatChar(maxLen - colNumToStr(i).length, ' ');

				totalWide += (maxLen + 3);
			}
		}

		totalTable += "\n";
		// 일단 기본적으로 최대 9999 번째 행까지 지원한다고 생각한다.
		for (int i = 0; i < maxRowSize; i++)
		{
			totalTable += repeatChar(totalWide, '-');
			totalTable += "\n" + std::to_string(i + 1);
			totalTable += repeatChar(4 - std::to_string(i + 1).length(), ' ');

			for (int j = 0; j < maxColSize; j++)
			{
				if (colMaxWide[j])
				{
					int maxLen = std::fmaxl(2, colMaxWide[j]);

					string s = "";
					if (dataTable[i][j])
					{
						s = dataTable[i][j]->stringify();
					}
					totalTable += " | " + s;
					totalTable += repeatChar(maxLen - s.length(), ' ');
				}
			}
			totalTable += "\n";
		}

		return totalTable;
	}

	HtmlTable::HtmlTable(int row, int col) : Table(row, col) {}

	string HtmlTable::PrintTable()
	{
		string s = "<table border='1' cellpadding='10'>";
		for (int i = 0; i < maxRowSize; i++)
		{
			s += "<tr>";
			for (int j = 0; j < maxColSize; j++)
			{
				s += "<td>";
				if (dataTable[i][j]) s += dataTable[i][j]->stringify();
				s += "</td>";
			}
			s += "</tr>";
		}
		s += "</table>";

		return s;
	}

	// CSVTable~~~~
	CSVTable::CSVTable(int row, int col) : Table(row, col) {}

	string CSVTable::PrintTable()
	{
		string s = "";
		for (size_t i = 0; i < maxRowSize; i++)
		{
			for (size_t j = 0; j < maxColSize; j++)
			{
				if (j >= 1) s += ",";
				// CSV 파일 규칙에 따라 문자열에 큰따옴표가 포함되어 있다면"" 로
				// 치환하다.
				string temp;
				if (dataTable[i][j]) temp = dataTable[i][j]->stringify();
			}
		}
	}
}

#endif

int main()
{
	MyExcel::TxtTable table(5, 5);
	std::ofstream out("test.txt");

	table.RegCell(new MyExcel::Cell("Hello~", 0, 0, &table), 0, 0);
	table.RegCell(new MyExcel::Cell("C++", 0, 1, &table), 0, 1);

	table.RegCell(new MyExcel::Cell("Programming", 1, 1, &table), 1, 1);
	std::cout << std::endl << table;

	out << table;
}
