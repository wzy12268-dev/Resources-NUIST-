/*
请改正程序中指定位置的错误，使程序的输出结果如下： 
45123
65123

注意：只允许修改注释“ERROR”的下一行，不得改动程序中的其他内容，也不允许增加或删减语句。
源程序：
*/
#include<iostream>
using namespace std;

class BaseClass {
	public:
		int x;
};

/**********ERROR**********/
class ClassA:public BaseClass 
{	protected:
		int y;
};

class ClassB:public BaseClass {
	protected:
		int z;
};


/**********ERROR**********/
class Derived:public ClassA,public ClassB 
{	public:
		int x;
		Derived() { 
			x=1,y=2,z=3;
/**********ERROR**********/
			ClassA::x=4,ClassB::x=5; 
		}
	void Display() {
		cout<<ClassA::x<<ClassB::x<<x<<y<<z<<endl;
	}
};

int main() {
	Derived d;
	d.Display();
/**********ERROR**********/
	d.ClassA::x=6; 
	d.Display();
	return 0;
}
