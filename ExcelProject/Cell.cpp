#include "Cell.h"
#include "Stack.h"
#include "Table.h"
#include "Vector.h"
#include <time.h>

// Cell~~~~
Cell::Cell(int x, int y, Table* table) : x(x), y(y), table(table) {}

// StringCell~~~~
StringCell::StringCell(string data, int x, int y, Table* t) : data(data), Cell(x, y, t) {}
string StringCell::stringify() { return data; }
int StringCell::toNumeric() { return 0; }
/////

// NumberCell
NumberCell::NumberCell(int data, int x, int y, Table* t) : data(data), Cell(x, y, t) {}
string NumberCell::stringify() { return std::to_string(data); }
int NumberCell::toNumeric() { return data; }
/////

ExprCell::ExprCell(string data, int x, int y, Table* t) : data(data), Cell(x, y, t) {}
string ExprCell::stringify() { return data; }

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

	for (int i = 0; i < exp_vec.size(); i++)
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