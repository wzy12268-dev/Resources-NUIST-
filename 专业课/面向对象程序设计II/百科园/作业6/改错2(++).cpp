/*
请改正程序中指定位置的错误，使程序的输出结果如下：
x-y=-1
x+y=7

注意：只允许修改注释"ERROR"的下一行，不得改动程序中的其他内容，也不允许增加或删减语句。
源程序清单：
*/
#include <iostream>  
using namespace std; 
 
/**********ERROR**********/
//Test
class Test
{		const int x;

/**********ERROR**********/
		//int y;
		static int y;
	public: 
		Test(int i,int j): x(i) { y=j; } 

/**********ERROR**********/
		//int display()
		void display() const                                    // 注意修改为void 
		{ cout<<"x+y="<<x+y<<endl; }  
		void display() { cout<<"x-y="<<x-y<<endl; }  
}; 
int Test::y=0;
 
int main() {  
	Test t1(1,2); 
	t1.display();
	const Test t2(3,4); 

/**********ERROR**********/
	//t2.display() const;
	t2.display(); 
	return 0; 
}
