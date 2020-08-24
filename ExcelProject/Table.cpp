#include <iostream>
#include "Table.h"
#include "Cell.h"

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
			totalTable += repeatChar(maxLen - colNumToStr(i).length(), ' ');

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
	for (int i = 0; i < maxRowSize; i++)
	{
		for (int j = 0; j < maxColSize; j++)
		{
			if (j >= 1) s += ",";
			// CSV 파일 규칙에 따라 문자열에 큰따옴표가 포함되어 있다면"" 로
			// 치환하다.
			string temp;
			if (dataTable[i][j]) temp = dataTable[i][j]->stringify();
		}
	}

	return s;
}