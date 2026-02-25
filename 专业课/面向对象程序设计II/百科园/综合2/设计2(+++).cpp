/*
日期（Date）类成员声明如下：
class Date { 
	public:		
		Date(int yy=2023,int mm=1,int dd=1); //构造函数，yy, mm, dd分别用于初始化日期的年、月、日 
		void setDate(int newY,int newM,int newD); // 设置日期的年、月、日为newY, newM, newD 
		void showDate() const; // 显示（输出）当前日期，输出格式为"年-月-日" 
	protected:
		int year,month,day;  // 日期的年、月、日 
};
请参照注释，完成以下任务：
（1）完成Date类的定义。
（2）以成员函数形式重载“==”运算符，用于判断两个日期对象是否相等（对应数据值相等），若相等则返回true，否则返回false。
（3）重载“>>”运算符，用于日期对象的输入（以“年 月 日”的格式输入）。

注意：部分源程序已给出，仅允许在注释“Begin”和“End”之间补全代码，不得改动其他已有的任何内容。

测试样例：
输入：
2023 4 10
2023 5 10
2023 4 10
输出：
d1的值为：2023-4-10
d2的值为：2023-5-10
d1和d2不是同一天
重置后d2的值为：2023-4-10
d1和d2是同一天

试题程序: 
*/
#include <iostream>
#include<fstream>
using namespace std;

/*******Begin*******/
class Date { 
	public:		
		Date(int yy=2023,int mm=1,int dd=1):year(yy),month(mm),day(dd){
		} 
		void setDate(int newY,int newM,int newD){
			this->year=newY;
			this->month=newM;
			this->day=newD;
		}
		void showDate() const; // 显示（输出）当前日期，输出格式为"年-月-日" 
		
		bool operator ==(const Date &);
		friend istream &operator >>(istream &,Date &);
	protected:
		int year,month,day;  // 日期的年、月、日 
};

bool Date::operator ==(const Date &d){
	if(this->year==d.year&&this->month==d.month&&this->day==d.day) return true;
	else return false;
}
istream &operator >>(istream &in,Date &d){
	in>>d.year>>d.month>>d.day;
	return in;
}

/*******End*********/

void Date::showDate() const { // 显示（输出）当前日期，输出格式为"年-月-日" 
	cout<<this->year<<"-"<<this->month<<"-"<<this->day<<endl;
}

int main() {
	int y,m,d;
	Date d1,d2;
	cin>>d1>>d2;	
	cout<<"d1的值为："; d1.showDate();
	cout<<"d2的值为："; d2.showDate();
	if(d1==d2) cout<<"d1和d2是同一天"<<endl; 
	else cout<<"d1和d2不是同一天"<<endl;
	cin>>y>>m>>d;
	d2.setDate(y,m,d);
	cout<<"重置后d2的值为："; d2.showDate();
	if(d1.operator ==(d2)) cout<<"d1和d2是同一天"<<endl; 
	else cout<<"d1和d2不是同一天"<<endl;
		
	ifstream in1("8.2.2.5_2-s2_in.dat");
	ofstream out1("8.2.2.5_2-s2_out.dat");
	streambuf *cinbackup;
	streambuf *coutbackup;
	cinbackup=cin.rdbuf(in1.rdbuf());
	coutbackup=cout.rdbuf(out1.rdbuf());
	while(cin>>d1>>d2) {
		cout<<"d1的值为："; d1.showDate();
		cout<<"d2的值为："; d2.showDate();
		if(d1==d2) cout<<"d1和d2是同一天"<<endl; 
		else cout<<"d1和d2不是同一天"<<endl;
		cin>>y>>m>>d;
		d2.setDate(y,m,d);
		cout<<"重置后d2的值为："; d2.showDate();
		if(d1.operator ==(d2)) cout<<"d1和d2是同一天"<<endl; 
		else cout<<"d1和d2不是同一天"<<endl;
		cout<<endl; 
	}
	cin.rdbuf(cinbackup);
	cout.rdbuf(coutbackup);
	in1.close();
	out1.close();	
	return 0;
}
/*
class Date{
	public:
		Date(int yy=2023,int mm=1,int dd=1);
		void setDate(int newY,int newM,int newD);
		void showDate() const;
		bool operator ==(const Date &) const;
		friend istream &operator >>(istream &,Date &);
	protected:
		int year,month,day;
}; 
Date::Date(int yy,int mm,int dd){
	this->year=yy;
	this->month=mm;
	this->day=dd;
}
void Date::setDate(int newY,int newM,int newD){
	this->year=newY;
	this->month=newM;
	this->day=newD;
}
bool Date::operator ==(const Date &d) const{
	if(year==d.year&&month==d.month&&day==d.day) return true;
	else return false;
}
istream &operator >>(istream &in,Date &d){
	in>>d.year>>d.month>>d.day;
	return in;
}
*/ 
