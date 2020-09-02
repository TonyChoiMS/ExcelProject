#pragma once
#include <iostream>

class Complex
{
private:
	double real;
	double img;

public:
	Complex(double real, double img) : real(real), img(img) {}
	Complex(const Complex& c) { real = c.real, img = c.img; }

	Complex operator+(const Complex& c);
	Complex operator-(const Complex& c);
	Complex operator*(const Complex& c);
	Complex operator/(const Complex& c);

	void Println()
	{
		std::cout << "( " << real << ", " << img << " )" << std::endl;
	}
};

