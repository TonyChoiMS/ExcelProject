
#include <iostream>
#include <string>
#include <fstream>

#ifndef UTILS_H
#define UTILS_H

using std::string;

namespace MyExcel
{
	templete <typename T>
	class Vector
	{
		T* data;
		int capacity;
		int length;

	public:
		Vector(int n = 1) : data(new T[n]), capacity(n), length(0) {}
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

	public:
		Cell(int x, int y, Table* table);
		~Cell();

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

		Vector exp_vec;
		
		// 연산자 우선 순위를 반환합니다.
		int precedence(char c);

		// 수식을 분석합니다.
		void parse_expression();

	public:
		ExprCell(string data, int x, int y, Table* t);

		string stringify() override;
		int toNumeric() override;
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
	Vector::~Vector()
	{
		if (data)
			delete[] data;
	}

	void Vector::pushBack(T s)
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

	T Vector::operator[](int i)
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

	int Vector::size() { return length; }

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
	Cell::Cell(int x, int y, Table* table) : x(x), y(y), table(table) {}

	// StringCell~~~~
	StringCell::StringCell(string data, int x, int y, Table* t) : data(data), Cell(x, y, t){}
	string StringCell::stringify() { return data; }
	int StringCell::toNumeric() { return 0; }
	/////

	// NumberCell
	NumberCell::NumberCell(int data, int x, int y, Table* t) : data(data), Cell(x, y, t) {}
	string NumberCell::stringify() { return std::to_string(data); }
	int NumberCell::toNumeric() { return data; }
	/////

	// DateCell
	string DateCell::stringify() 
	{
		char buf[50];
		tm temp;
		localtime_s(&temp, &data);

		return string(buf);
	}

	int DateCell::toNumeric() { return static_cast<int>(data); }

	DateCell::DateCell(string s, int x, int y, Table* t) : Cell(x, y, t)
	{
		//  입력받는 Date 형식은 항상 yyyy-mm-dd  꼴이라 가정한다.
		int year = atoi(s.c_str());
		int month = atoi(s.c_str() + 5);
		int day = atoi(s.c_str() + 8);

		tm timeinfo;

		timeinfo.tm_year = year - 1900;
		timeinfo.tm_mon = month - 1;
		timeinfo.tm_mday = day;
		timeinfo.tm_hour = 0;
		timeinfo.tm_min = 0;
		timeinfo.tm_sec = 0;

		data = mktime(&timeinfo);
	}
	/////

	// ExprCell
	int ExprCell::toNumeric()
	{
		double result = 0;
		NumStack stack;

		for (size_t i = 0; i < exp_vec.size(); i++)
		{
			string s = exp_vec[i];

			// 셀일 경우
			if (isalpha(s[0]))
			{
				stack.push(table->toNumeric(s));
			}
			else if (isdigit(s[0]))
			{
				stack.push(atoi(s.c_str()));
			}
			else
			{
				double y = stack.pop();
				double x = stack.pop();

				switch (s[0])
				{
				case '+':
					stack.push(x + y);
					break;
				case '-':
					stack.push(x - y);
					break;
				case '*':
					stack.push(x * y);
					break;
				case '/':
					stack.push(x / y);
					break;
				
				}
			}
		}

		return stack.pop();
	}

	int ExprCell::precedence(char c)
	{
		switch (c)
		{
		case '(':
		case '[':
		case '{':
			return 0;

		case '+':
		case '-':
			return 1;

		case '*':
		case '/':
			return 2;
		}

		return 0;
	}

	void ExprCell::parse_expression()
	{
		Stack stack;

		// 마지막에 스택에 남아있는 연산자들을 모두 pop해야되기 떄문에
		// 수식 전체를 ()로 둘러 사서 exp_vec에 남아있는 연산자들이 push되게
		// 해줍니다.
		data.insert(0, "(");
		data.push_back(')');

		for (size_t i = 0; i < data.length(); i++)
		{
			if (isalpha(data[i]))				// 셀 이름의 경우 첫번째 글자가 알파벳이다.
			{
				exp_vec.pushBack(data.substr(i, 2));
				i++;
			}
			else if (isdigit(data[i]))			// 첫번째 글자가 숫자라면 정수 데이터
			{
				exp_vec.pushBack(data.substr(i, 1));
			}
			else if (data[i] == '(' || data[i] == '[' || data[i] == '{')
			{
				//parenthesis
				stack.push(data.substr(i, 1));
			}
			else if (data[i] == ')' || data[i] == ']' || data[i] == '}')
			{
				string t = stack.pop();
				while (t != "(" && t != "[" && t != "{")
				{
					exp_vec.pushBack(t);
					t = stack.pop();
				}
			}
			else if (data[i] == '+' || data[i] == '-' || data[i] == '*' || data[i] == '/')
			{
				while (!stack.isEmpty() && precedence(stack.peek()[0] >= precedence(data[i])))
				{
					exp_vec.pushBack(stack.pop());
				}
				stack.push(data.substr(i, 1));
			}
		}
	}

	////

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

	int parseUserInput(string s)
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
			current_table->reg_cell(new StringCell(rest, row, col, current_table), row, col);
		}
		else if (command == "setn")
		{
			current_table->reg_cell(new NumberCell(atoi(rest.c_str()), row, col, current_table), row, col);
		}
		else if (command == "setd")
		{
			current_table->reg_cell(new DateCell(rest, row, col, current_table), row, col);
		}
		else if (command == "out")
		{
			ofstream out(to);
			out << *current_table;
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
		std::getline(cin, s);

		while (parseUserInput(s))
		{
			std::cout << *current_table << std::endl << ">> ";
			getline(cin, s);
		}
	}

	/////
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
