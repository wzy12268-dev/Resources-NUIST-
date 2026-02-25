/*
时钟（Clock）类成员声明如下：
class Clock	{	
	public:	
		Clock(int h=0,int m=0,int s=0); //构造函数，h, m, s分别用于初始化时钟的时、分、秒
		void setTime(int newH,int newM,int newS); // 设置时钟的时、分、秒 
	protected:	
		int hour,minute,second; // 时钟的时、分、秒（24小时制）
};
请根据注释，在完善Clock类定义的基础上，完成以下运算符重载：
（1）以成员函数形式重载前置“++”运算，实现时钟秒数（second）加1的操作。
（2）将运算符“==”重载为非成员函数形式，用于判断两个时钟对象是否相等（若相等则返回true，否则返回false）。
（3）重载“<<”运算符，用于时钟对象的输出，输出格式为“时:分:秒”。

注意：部分源程序已给出，仅允许在注释“Begin”和“End”之间补全代码，不得改动其他已有的任何内容。

测试样例：
输入：
0 0 0
0 0 0
输出：
时钟c1：0:0:1
时钟c2：0:0:1
时钟c1与c2相等！
重置后时钟c2：0:0:0
时钟c1与c2不相等！

试题程序: 
*/
#include<iostream>
#include<fstream>
using namespace std;

/*******Begin*******/
class Clock	{	
	public:	
		Clock(int h=0,int m=0,int s=0){ //构造函数，h, m, s分别用于初始化时钟的时、分、秒
			this->hour=h;
			this->minute=m;
			this->second=s; 
		}
		void setTime(int newH,int newM,int newS) // 设置时钟的时、分、秒 
		{
			this->hour=newH;
			this->minute=newM;
			this->second=newS; 
		}
		Clock& operator ++();
		friend bool operator ==(const Clock &,const Clock &);
		friend ostream & operator <<(ostream &, const Clock &);
	protected:	
		int hour,minute,second; // 时钟的时、分、秒（24小时制）
};

Clock & Clock::operator ++(){
	second++;
	if(second>=60){
		second-=60;
		minute++;
		if(minute>=60){
			minute-=60;
			hour=(hour+1)%24;
		}
	}
	return *this;
}

bool operator ==(const Clock &c1,const Clock &c2){
	if(c1.hour==c2.hour&&c1.minute==c2.minute&&c1.second==c2.second){
		return true;
	}else return false;
}

/*******End*********/

ostream & operator <<(ostream &out, const Clock &c) {	// 运算符<<重载
	out << c.hour << ":" << c.minute << ":" << c.second;
	return out;
}

int main()
{
	int h,m,s;
	cin>>h>>m>>s;
	Clock c1(h,m,s),c2=++c1;	
	cout<<"时钟c1："<<c1<<endl;
	cout<<"时钟c2："<<c2<<endl;
	if(operator ==(c1,c2)) cout<<"时钟c1与c2相等！"<<endl;
	else cout<<"时钟c1与c2不相等！"<<endl; 
	cin>>h>>m>>s;
	c2.setTime(h,m,s); 
	cout<<"重置后时钟c2："<<c2<<endl;
	if(c1==c2) cout<<"时钟c1与c2相等！"<<endl;
	else cout<<"时钟c1与c2不相等！"<<endl; 

	ifstream in1("8.2.2.5_1-s1_in.dat");
	ofstream out1("8.2.2.5_1-s1_out.dat");
	streambuf *cinbackup;
	streambuf *coutbackup;
	cinbackup=cin.rdbuf(in1.rdbuf());
	coutbackup=cout.rdbuf(out1.rdbuf());
	while(cin>>h>>m>>s) {
		Clock c1(h,m,s),c2=c1.operator ++(); 
		cout<<"时钟c1："<<c1<<endl;
		cout<<"时钟c2："<<c2<<endl;
		if(operator ==(c1,c2)) 
			cout<<"时钟c1与c2相等！"<<endl;
		else cout<<"时钟c1与c2不相等！"<<endl; 
		cin>>h>>m>>s;
		c2.setTime(h,m,s); 
		cout<<"重置后时钟c2："<<c2<<endl;
		if(c1==c2) cout<<"时钟c1与c2相等！"<<endl;
		else cout<<"时钟c1与c2不相等！"<<endl;
		cout<<endl;
	}
	cin.rdbuf(cinbackup);
	cout.rdbuf(coutbackup);
	in1.close();
	out1.close();
 	return 0;
 }
