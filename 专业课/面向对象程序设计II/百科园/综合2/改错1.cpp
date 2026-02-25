/*
请改正程序中指定位置的错误，使程序的输出结果如下：
The computer id is 101
The type of monitor is A

注意：只允许修改注释"ERROR"的下一行，不得改动程序中的其他内容，也不允许增加或删减语句。
源程序清单：
*/
#include<iostream>
using namespace std;

class Monitor {
	public:
		Monitor(char t) { type=t; }

/**********ERROR**********/
		void display() const 
		{ cout<<"The type of monitor is "<<type<<endl; }
	private:
		char type;
};

class Computer {
	public:

/**********ERROR**********/
		Computer(int i, char c):mon(c)
		{ id=i; }

/**********ERROR**********/
		void Display() const
		{	cout<<"The computer id is "<<id<<endl; 

/**********ERROR**********/
			mon.display(); 
		}
	private:
		int id;
		Monitor mon;
};

int main() {
	const Computer myComputer(101, 'A');
	myComputer.Display(); 
	return 0;
}
