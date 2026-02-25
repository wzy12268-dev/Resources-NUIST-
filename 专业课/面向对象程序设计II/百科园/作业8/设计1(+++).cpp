/*
复数（Complex）类定义如下：  
class Complex {	
	public:	
		Complex(int r = 0, int i = 0): real(r), imag(i) { }	// 构造函数
	private:
		int real;	// 复数实部
		int imag;  // 复数虚部
};
基于上述定义，请完成以下运算符重载：
（1）以非成员函数形式重载前置“++”运算符，使实部和虚部分别加1。
（2）将运算符“==”重载为成员函数形式，实现判断两个复数是否相等（若相等则返回true，否则返回false）。
（3）重载“<<”运算符，用于复数对象的输出（格式为“(real, imag)”）。
注意：部分源程序已给出，仅允许在注释“Begin”和“End”之间补全代码，不得改动其他已有的任何内容。

测试样例：
输入：
1 2 2 3
输出：
c1的值为：(1, 2)
c2的值为：(2, 3)
c3的值为：(0, 0)
c1!=c2

c3=++c1的值为：(2, 3)
c1==c2

试题程序: 
*/
#include <iostream>
#include<fstream>
using namespace std;

/*******Begin*******/
class Complex {	
	public:	
		Complex(int r = 0, int i = 0): real(r), imag(i) { }	// 构造函数
		friend ostream & operator <<(ostream &, const Complex &);
		bool operator ==(const Complex &);
		friend Complex &operator ++(Complex &);
	private:
		int real;	// 复数实部
		int imag;  // 复数虚部
};
bool Complex::operator ==(const Complex &c){
	if(this->real==c.real&&this->imag==c.imag) return true;
	else return false;
}
Complex &operator ++(Complex &c){
	c.real++;
	c.imag++;
	return c;
}
/*******End*********/

ostream & operator <<(ostream &out, const Complex &c) { //运算符<<重载函数实现
	out << "(" << c.real << ", " << c.imag << ")";
	return out;
}

int main() {
	int r1,i1,r2,i2;
	cin>>r1>>i1>>r2>>i2;
	Complex c1(r1,i1),c2(r2,i2),c3;
	cout<<endl;
	cout<<"c1的值为："<<c1<<endl;
	cout<<"c2的值为："<<c2<<endl;
	cout<<"c3的值为："<<c3<<endl;
	if(c1==c2) cout<<"c1==c2"<<endl; 
	else cout<<"c1!=c2"<<endl;
	cout<<endl;
	c3=++c1;
	cout<<"c3=++c1的值为："<<c3<<endl;
	if(c1==c2) cout<<"c1==c2"<<endl; 
	else cout<<"c1!=c2"<<endl;	
	ifstream in1("8.2.1.1-s3.in");
	ofstream out1("8.2.1.1-s3.out");
	streambuf *cinbackup;
	streambuf *coutbackup;
	cinbackup=cin.rdbuf(in1.rdbuf());
	coutbackup=cout.rdbuf(out1.rdbuf());
	while(cin>>r1>>i1>>r2>>i2) {
		Complex c1(r1,i1),c2(r2,i2),c3;
		cout<<"c1的值为："<<c1<<endl;
		cout<<"c2的值为："<<c2<<endl;
		cout<<"c3的值为："<<c3<<endl;
		if(c1==c2) cout<<"c1==c2"<<endl; 
		else cout<<"c1!=c2"<<endl;
		c3=operator ++(c1);
		cout<<"c3=++c1的值为："<<c3<<endl;
		if(c1.operator ==(c2)) cout<<"c1==c2"<<endl; 
		else cout<<"c1!=c2"<<endl;
		cout<<endl;
	}
	cin.rdbuf(cinbackup);
	cout.rdbuf(coutbackup);
	in1.close();
	out1.close();
	return 0;
}
/*
class Complex {	
	public:	
		Complex(int r = 0, int i = 0): real(r), imag(i) { }	// 构造函数
		friend Complex& operator ++(Complex &c);
		bool operator ==(const Complex &c2) const;
		friend ostream & operator <<(ostream &,const Complex &); 
	private:
		int real;	// 复数实部
		int imag;  // 复数虚部
};
Complex& operator ++(Complex &c){
	c.real++;
	c.imag++;
	return c;
}

bool Complex::operator ==(const Complex &c2) const{
	if(this->real==c2.real&&this->imag==c2.imag) return true;
	else return false;
}
*/ 
