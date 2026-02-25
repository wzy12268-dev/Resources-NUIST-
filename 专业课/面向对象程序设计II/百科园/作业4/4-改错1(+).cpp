/*
下面的程序实现复数相加运算，请改正程序中指定位置的错误，使程序的输出结果如下： 
c3=(0, 0i)
c3=c1+c2=(4.7, 1.4i)
注意：只允许修改注释“ERROR”的下一行，不得改动程序中的其他内容，也不允许增加或删减语句。
源程序清单：
*/
#include <iostream>
using namespace std;

/**********ERROR**********/
//Complex
class Complex
{
	public:

/**********ERROR**********/
		//void Complex(double, double)
		Complex(double r=0, double i=0)
		{ real=r; imag=i; }
		Complex add(Complex &);
		void display() {
			cout<<"("<<real<<", "<<imag<<"i)"<<endl;
		}
	private: 
		double real, imag;
};

/**********ERROR**********/
//Complex::add(Complex &c)
Complex Complex::add(Complex &c)
{	
	return Complex(real+c.real,imag+c.imag);
}
int main() {
	Complex c1(1.2,2.4),c2(3.5,-1),c3;
	cout<<"c3="; c3.display();

/**********ERROR**********/
	//c3=c1+c2;
	c3=c1.add(c2);
	cout<<"c3=c1+c2="; c3.display();
	return 0;
}
