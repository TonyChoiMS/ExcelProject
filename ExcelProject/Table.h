#pragma once
#include <iostream>
#include <string>

using std::string;

class Cell;

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


