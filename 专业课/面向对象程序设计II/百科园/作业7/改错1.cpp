/*
请改正程序中指定位置的错误，使程序的输出结果如下： 
Base1
Base2
3, 1, 4

注意：只允许修改注释"ERROR"的下一行，不得改动程序中的其他内容，也不允许增加或删减语句。
源程序清单：
*/
#include <iostream>
using namespace std;
class Base1 {
	public: 
		Base1(int x) {
			b1=x;
			cout<<"Base1"<<endl;
		}
	protected: 
		int b1;  
};
class Base2 {
	public:
		Base2() {
			b2++;
			cout<<"Base2"<<endl;
		}
	protected:
		static int b2; 
};

/**********ERROR**********/
//static int b2=0; 
int Base2::b2=0;
/**********ERROR**********/
//class Derived:public Base1,Base2 
class Derived:public Base1,public Base2
{	public:

/**********ERROR**********/
		//Derived(int a,int b):b1=a    
		Derived(int a,int b):Base1(a)
		{ d=b; }  // 形参a,b分别用于初始化数据成员b1,d

/**********ERROR**********/
		//void Display() 
		void Display() const
		{ cout<<b1<<", "<<b2<<", "<<d<<endl; }
	private:
		int d;
};
int main() {
	const Derived d(3,4);
	d.Display(); 
	return 0;
}
