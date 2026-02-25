/*
点（Point）类成员如下： 
（1）公有成员：
Point(float xx, float yy)   // 构造函数，初始化点的x, y坐标
void moveTo(float newX, float newY)  // 将点的x, y坐标移动到newX, newY
（2）私有成员：
float x, y   // 点的横坐标，纵坐标
在此基础上，定义正方形（Square）类，其成员如下：
（1）公有成员：
Square(float x=0.0,float y=0.0,float len=1.0)  // 构造函数，初始化所有数据成员，其中(x,y)为左下角位置，len为边长
void resetSquare(float newX, float newY, float newLen) // 重置正方形左下角坐标为(newX, newY)，边长为newLen
double getLen() // 返回正方形的边长
double getCircumference() // 返回正方形的周长
bool isEqual(Square &s) // 判断与另一个正方形是否大小相等
（2）私有成员：
Point p   // 正方形左下角位置
float length  // 正方形的边长
double circumference  // 正方形的面积，周长
请根据上述说明，完成Point，Square两个类的定义。

注意：部分源程序给出，仅允许在注释“Begin”和“End”之间填写内容，不得改动main函数和其他已有的任何内容。
试题程序：
*/
#include<iostream>
#include<fstream>
using namespace std;

/*******Begin*******/
class Point{
	public:
		Point(float xx, float yy){
			x=xx;
			y=yy;
		}
		void moveTo(float newX, float newY){
			x=newX;
			y=newY; 
		}
	private:
		float x, y;  // 点的横坐标，纵坐标
}; 
class Square{
	public:
		Square(float x=0.0,float y=0.0,float len=1.0):p(x,y){
			length=len;
			circumference=4*length;
		} 
		void resetSquare(float newX, float newY, float newLen)
		{
			p.moveTo(newX,newY);
			length=newLen;
			circumference=4*length;
		}
		double getLen(){
			return length;
		}
		double getCircumference()
		{
			return circumference;
		}
		bool isEqual(Square &s)
		{
			if(length==s.length) return true;
			else return false;
		}
	private:
		Point p;  // 正方形左下角位置
		float length;  // 正方形的边长
		double circumference;  // 正方形的面积，周长
};





/*******End*********/

int main() {
	float x,y,len;
	cin>>x>>y>>len;
	Square s1(x,y,len),s2;
	cout<<"s1 边长: "<<s1.getLen()<<", 周长: "<<s1.getCircumference()<<endl;
	cout<<"s2 边长: "<<s2.getLen()<<", 周长: "<<s2.getCircumference()<<endl;
	cout<<"是否相等: "<<s2.isEqual(s1)<<endl;
		
	cin>>x>>y>>len;
	s2.resetSquare(x,y,len);
	cout<<"重置后:\ns1 边长: "<<s1.getLen()<<", 周长: "<<s1.getCircumference()<<endl;
	cout<<"s2 边长: "<<s2.getLen()<<", 周长: "<<s2.getCircumference()<<endl;
	cout<<"是否相等: "<<s2.isEqual(s1)<<endl;
	ifstream in1("4.2.3_2-2_in.dat");
	ofstream out1("4.2.3_2-2_out.dat");
	while(in1>>x>>y>>len)
	{
		Square s1(x,y,len),s2;
		out1<<"s1 边长: "<<s1.getLen()<<", 周长: "<<s1.getCircumference()<<endl;
		out1<<"s2 边长: "<<s2.getLen()<<", 周长: "<<s2.getCircumference()<<endl;
		out1<<"是否相等: "<<s2.isEqual(s1)<<endl;	
		in1>>x>>y>>len;
		s2.resetSquare(x,y,len);
		out1<<"重置后:\ns1 边长: "<<s1.getLen()<<", 周长: "<<s1.getCircumference()<<endl;
		out1<<"s2 边长: "<<s2.getLen()<<", 周长: "<<s2.getCircumference()<<endl;
		out1<<"是否相等: "<<s2.isEqual(s1)<<endl<<endl;
	}
	in1.close();
	out1.close();
	return 0;
}
