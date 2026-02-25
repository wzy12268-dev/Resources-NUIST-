/*
时钟（Clock）类用于模拟时钟的常用功能，其成员如下： 
（1）公有成员：
Clock(int hh=0,int mm=0,int ss=0) // 构造函数，hh,mm,ss分别用于初始化时钟的时、分、秒
void setTime(int newH,int newM,int newS)  // 设置时钟的时、分、秒数据
void add(int n)  // 时钟秒数加n
void dec()  // 时钟秒数减1
int dist(Clock &c)  // 与另一个时钟对象的时间差值，单位为秒
void showTime()  // 显示（输出）当前时钟数据
（2）私有成员：
int hour, minute, second  // 时钟的时、分、秒（24小时制）

请根据上述说明，完成Clock类的定义。
测试样例：
输入：
0 0 0
3675
13 10 30
输出：
时钟c1: 0:0:0
时钟c2: 0:0:0
时钟c1与c2相差0秒
时钟c1: 1:1:15
时钟c2: 23:59:59
时钟c1与c2相差82724秒
重置后时钟c1: 13:10:30
注意：部分源程序已给出，仅允许在注释“Begin”和“End”之间补全代码，不得改动其他已有的任何内容。
试题程序：
*/
#include<iostream>
#include<fstream>
#include<cmath>
using namespace std;

/*******Begin*******/
class Clock{
	public:
		Clock(int hh=0,int mm=0,int ss=0){
			hour=hh;
			minute=mm;
			second=ss;
		}
		void setTime(int newH,int newM,int newS){
			hour=newH;
			minute=newM;
			second=newS;
		} 
		void add(int n);
		void dec();
		int dist(Clock &c); 
		void showTime(); 
	public:
		int hour, minute, second;
		
}; 
void Clock::add(int n){
	second+=n;
	minute+=second/60;
	second%=60;
	hour+=minute/60;
	minute%=60;
	hour%=24;
}
void Clock::dec(){
	second--;
	if(second<0)
	{
		second+=60;
		minute--;
		if(minute<0)
		{
			minute+=60;
			hour--;
			if(hour<0)
			{
				hour+=24;
			}
		}
	}
}
int Clock::dist(Clock &c){
	return abs((hour*3600+minute*60+second)-(c.hour*3600+c.minute*60+c.second));
}
/*******End*********/

void Clock::showTime() {
	cout<<hour<<":"<<minute<<":"<<second<<endl;
}

int main() {
	int h,m,s,n;
	cin>>h>>m>>s;
	Clock c1(h,m,s),c2;
	cout<<"时钟c1: "; c1.showTime();
	cout<<"时钟c2: "; c2.showTime();
	cout<<"时钟c1与c2相差"<<c1.dist(c2)<<"秒"<<endl;
	cin>>n;
	c1.add(n); c2.dec();
	cout<<"时钟c1: "; c1.showTime();
	cout<<"时钟c2: "; c2.showTime();
	cout<<"时钟c1与c2相差"<<c1.dist(c2)<<"秒"<<endl;
	cin>>h>>m>>s;
	c1.setTime(h,m,s);
	cout<<"重置后时钟c1: "; c1.showTime();	
	ifstream in1("4.1.5_1-5.in");
	ofstream out1("4.1.5_1-5.out");
	streambuf *cinbackup;
	streambuf *coutbackup;
	cinbackup=cin.rdbuf(in1.rdbuf());
	coutbackup=cout.rdbuf(out1.rdbuf());
	while(cin>>h>>m>>s) {
		Clock c1(h,m,s),c2;
		cout<<"时钟c1: "; c1.showTime();
		cout<<"时钟c2: "; c2.showTime();
		cout<<"时钟c1与c2相差"<<c1.dist(c2)<<"秒"<<endl;
		cin>>n;
		c1.add(n); c2.dec();
		cout<<"时钟c1: "; c1.showTime();
		cout<<"时钟c2: "; c2.showTime();
		cout<<"时钟c1与c2相差"<<c1.dist(c2)<<"秒"<<endl;
		cin>>h>>m>>s;
		c1.setTime(h,m,s);
		cout<<"重置后时钟c1: "; c1.showTime();
		cout<<endl;
	}
	cin.rdbuf(cinbackup);
	cout.rdbuf(coutbackup);
	in1.close();
	out1.close();
	return 0;
}
