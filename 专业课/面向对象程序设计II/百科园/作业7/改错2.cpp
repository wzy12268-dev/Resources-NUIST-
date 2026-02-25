/*
请改正程序中指定位置的错误，使程序的输出结果如下： 
classBase
classD1
classD2
classD3
finBase
finD1
finD2

注意：只允许修改注释"ERROR"的下一行，不得改动程序中的其他内容，也不允许增加或删减语句。
源程序：
*/
#include <iostream>
using namespace std;
class Base{

/**********ERROR**********/
	//protected:
	public:
		Base(){cout<<"classBase"<<endl;}
		void f(){cout<<"finBase"<<endl;} 
};

/**********ERROR**********/
//class D1:public Base
class D1:virtual public Base
{	public:
		D1(){cout<<"classD1"<<endl;}
		void f(){cout<<"finD1"<<endl;} 
};
class D2:virtual protected Base  
{	public:
		D2(){cout<<"classD2"<<endl;} 
		void f(){cout<<"finD2"<<endl;} 
};

/**********ERROR**********/
//class D3:public D1,D2
class D3:public D1,public D2
{	public:
		D3(){cout<<"classD3"<<endl;} 
};
int main() {
	D3 d;

/**********ERROR**********/
	//d.f();
	d.Base::f();
	d.Base::f();
	d.D1::f();
	d.D2::f();
	return 0;
}
