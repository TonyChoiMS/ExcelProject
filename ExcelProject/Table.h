#pragma once
#include <iostream>
#include <string>

using std::string;

class Cell;

class Table
{
protected:
	int maxRowSize, maxColSize;		// �� �� ���� �ִ� ũ��

	// �����͸� �����ϴ� ���̺�
	// Cell*�� �����ϴ� 2���� �迭
	Cell*** dataTable;

public:
	Table(int maxRowSize, int maxColSize);
	~Table();

	// ���ο� ���� row �� col ���� ����Ѵ�.
	void RegCell(Cell* c, int row, int col);

	// �ش� ���� �������� ��ȯ�Ѵ�.
	// s : �� �̸�(Ex. A3, B6�� ����)
	int toNumeric(const string& s);

	// �� �� �� ��ȣ�� ���� ȣ���Ѵ�.
	int toNumeric(int row, int col);

	// �ش� ���� ���ڿ��� ��ȯ�Ѵ�.
	string stringify(const string& s);
	string stringify(int row, int col);

	virtual string PrintTable() = 0;
};

class TxtTable : public Table
{
	string repeatChar(int n, char c);

	// ���ڷ� �� �� ��ȣ�� A, B, .... Z, AA, AB, ... �̷� ������ �Ű��ش�.
	string colNumToStr(int n);

public:
	TxtTable(int row, int col);

	// �ؽ�Ʈ�� ǥ�� �����ϰ� ������ش�.
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


