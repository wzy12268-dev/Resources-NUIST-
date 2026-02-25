/*
下面程序在定义分数（fract）类的基础上，重载复合赋值运算符“+=”和提取符“>>”。
请在程序的指定位置填入正确的内容，使程序得出正确的结果。
程序运行时，若依次输入：
3 4 
5 7
则输出结果为：
fr1: 41/28
fr2: 5/7 
注意：仅允许在指定的下划线处填写内容，并删除下划线编号，不允许增加或删减语句，也不得改动程序中的其他任何内容。
试题源程序：
*/
#include <iostream>
using namespace std;
class fract {
		int num; //分子
		int den; //分母
	public:
		fract(int n=0,int d=1):num(n),den(d) { }
		fract &operator +=(const fract&); 
		void show() const { cout<<num<<'/'<<den<<endl; } 

/**********FILL**********/
		friend istream &operator>>(istream &, fract &);
};

/**********FILL**********/
fract & fract::operator +=(const fract& f)
{	
	this->num=this->num*f.den+this->den*f.num;
	this->den*=f.den;

/**********FILL**********/
	return *this;
}
istream &operator >>(istream &in, fract &f) { 
	in>>f.num>>f.den;

/**********FILL**********/
	return in;
	//return cin; 
}
int main() {
	int n,d;
	cin>>n>>d;
	fract fr1(n,d),fr2;
	cin>>fr2;
	fr1+=fr2; 
	cout<<"fr1: "; fr1.show(); 
	cout<<"fr2: "; fr2.show(); 
	return 0;
}
