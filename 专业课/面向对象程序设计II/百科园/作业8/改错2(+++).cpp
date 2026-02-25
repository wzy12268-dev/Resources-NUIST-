/*
请改正程序中指定位置的错误，使程序的输出结果如下： 
f1 function of base
f2 function of base
f1 function of derive
f2 function of base

注意：只允许修改注释"ERROR"的下一行，不得改动程序中的其他内容，也不允许增加或删减语句。
源程序清单：
*/
#include <iostream>
using namespace std;

class base {

/**********ERROR**********/
	public:   
 
/**********ERROR**********/
		virtual void f1()
		{ cout<<"f1 function of base"<<endl; }  
		void f2() { cout<<"f2 function of base "<<endl; }
};

/**********ERROR**********/
class derive:public base
{	public: 
		void f1() { cout<<"f1 function of derive"<<endl; }
		void f2() { cout<<"f2 function of derive "<<endl; }
};

int main() { 

/**********ERROR**********/
	base *p;
	base obj1; 
	derive obj2;
	p=&obj1; p->f1(); p->f2();
	p=&obj2; p->f1(); p->f2();
	return 0;
}
