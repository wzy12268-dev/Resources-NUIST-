/*
下面程序在日期（Date）类定义的基础上，重载"=="运算符，用于判断两个日期对象是否相等（对应数据值相等），若相等则返回true，否则返回false；重载运算符">>"，用于以"年 月 日"的顺序输入日期对象。但程序不完整。
请将程序补充完整，使得程序运行时输出正确结果。例如，输入：
2022 12 28
2022 12 28
则输出结果为：
日期d1：2022-12-28
日期d2：2022-10-24
d1和d2不是同一天
重置后日期d2：2022-12-28
d1和d2是同一天

注意：仅允许在指定的下划线处填写代码，不得改动程序中的其他内容（需删除下划线编号）。
试题源程序：
*/
#include <iostream>
using namespace std;
class Date { 
	public:		
		Date(int yy=2022,int mm=10,int dd=24);  // 构造函数，yy,mm,dd分别用于初始化日期的年、月、日 
		void setDate(int newY,int newM,int newD);  // 设置日期的年、月、日为newY, newM, newD 
		void showDate() const {  // 显示（输出）当前日期，输出格式为"年-月-日" 
			cout<<this->year<<"-"<<this->month<<"-"<<this->day<<endl;
		}
		bool operator ==(const Date &) const;  // 运算符==重载 

/**********FILL**********/
		friend istream &operator >>(istream &, Date &);  // 运算符>>重载  
	private:
		int year,month,day;  // 日期的年、月、日 
};

/**********FILL**********/
Date::Date(int yy,int mm,int dd){  
	this->year=yy; this->month=mm; this->day=dd;
}
void Date::setDate(int newY,int newM,int newD) { 
	this->year=newY; this->month=newM; this->day=newD; 
}

/**********FILL**********/
bool Date::operator ==(const Date &d) const{  
	if(this->year==d.year && this->month==d.month && this->day==d.day) return true;  
	else return false; 
}
istream &operator >>(istream &in, Date &d) { 
	in>>d.year>>d.month>>d.day;

/**********FILL**********/
	return in; 
} 

int main() {
	int y,m,d;
	Date d1,d2;
	cin>>d1;	
	cout<<"日期d1："; d1.showDate();
	cout<<"日期d2："; d2.showDate();
	if(d1==d2) cout<<"d1和d2是同一天"<<endl; 
	else cout<<"d1和d2不是同一天"<<endl;
	cin>>y>>m>>d;
	d2.setDate(y,m,d);
	cout<<"重置后日期d2："; d2.showDate();
	if(d1==d2) cout<<"d1和d2是同一天"<<endl; 
	else cout<<"d1和d2不是同一天"<<endl;
	return 0;
}
