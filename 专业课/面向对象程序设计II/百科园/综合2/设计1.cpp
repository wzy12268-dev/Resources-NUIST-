/*
点（Point）类成员如下： 
（1）公有成员：
Point(float xx, float yy)   // 构造函数，初始化点的x, y坐标
float getX()   // 返回横坐标x
float getY()   // 返回纵坐标y
void moveTo(float newX, float newY)  // 将点的x, y坐标移动到newX, newY
（2）私有成员：
float x, y   // 点的横坐标x，纵坐标y
在此基础上，定义矩形（Rectangle）类，其成员如下：
（1）公有成员：
Rectangle(float x1=0.0,float y1=0.0,float x2=3.0,float y2=4.0) // 构造函数，初始化矩形的数据成员，其中(x1,y1)为左下角坐标，(x2,y2)为右上角坐标
void resetRect(float newX1,float newY1,float newX2,float newY2) // 重置矩形左下角p1(newX1,newY1)，右上角p2(newX2,newY2)
double getArea() // 返回矩形的面积
double getCircumference() // 返回矩形的周长
bool isSquare() // 判断是否为正方形
（2）私有成员：
Point p1,p2   // 矩形左下角p1和右上角p2
double area, circumference  // 矩形的面积，周长 
请根据上述说明，完成Point，Rectangle两个类的定义。

注意：部分源程序给出，仅允许在注释“Begin”和“End”之间填写内容，不得改动main函数和其他已有的任何内容。
试题程序：
*/
#include<iostream>
#include<fstream>
using namespace std;

/*******Begin*******/
class Point{
	private:
		float x,y;
	public:
		Point(float xx, float yy):x(xx),y(yy){
		}
		float getX(){
			return x; 
		}
		float getY(){
			return y;
		}
		void moveTo(float newX, float newY){
			x=newX;
			y=newY;
		}
}; 
class Rectangle{
	private:
		Point p1,p2;
		double area,circumference;
	public:
		Rectangle(float x1=0.0,float y1=0.0,float x2=3.0,float y2=4.0):p1(x1,y1),p2(x2,y2){
			area=(p2.getX()-p1.getX())*(p2.getY()-p1.getY());
			circumference=2*(p2.getX()-p1.getX()+p2.getY()-p1.getY());
		}
		void resetRect(float newX1,float newY1,float newX2,float newY2){
			p1.moveTo(newX1,newY1);
			p2.moveTo(newX2,newY2); 
			area=(p2.getX()-p1.getX())*(p2.getY()-p1.getY());
			circumference=2*(p2.getX()-p1.getX()+p2.getY()-p1.getY());
		}
		double getArea(){
			return area;
		}
		double getCircumference(){
			return circumference;
		}
		bool isSquare(){
			double x=p2.getX()-p1.getX();
			double y=p2.getY()-p1.getY();
			return x==y;
		}
		
};




/*******End*********/

int main() {
	float x1,y1,x2,y2;
	cin>>x1>>y1>>x2>>y2;
	Rectangle rect1(x1,y1,x2,y2),rect2;
	cout<<"rect1 面积: "<<rect1.getArea()<<" 周长: "<<rect1.getCircumference()<<endl;
	cout<<"是否正方形: "<<rect1.isSquare()<<endl;
	cout<<"rect2 面积: "<<rect2.getArea()<<" 周长: "<<rect2.getCircumference()<<endl;
	cout<<"是否正方形: "<<rect2.isSquare()<<endl;	
	cin>>x1>>y1>>x2>>y2;
	rect2.resetRect(x1,y1,x2,y2);
	cout<<"重置后:\nrect2 面积: "<<rect2.getArea()<<" 周长: "<<rect2.getCircumference()<<endl;
	cout<<"是否正方形: "<<rect2.isSquare()<<endl;
	
	ifstream in1("4.2.1_4-2_in.dat");
	ofstream out1("4.2.1_4-2_out.dat");
	while(in1>>x1>>y1>>x2>>y2) {
		Rectangle rect1(x1,y1,x2,y2),rect2;
		out1<<"rect1 面积: "<<rect1.getArea()<<" 周长: "<<rect1.getCircumference()<<endl;
		out1<<"是否正方形: "<<rect1.isSquare()<<endl;
		out1<<"rect2 面积: "<<rect2.getArea()<<" 周长: "<<rect2.getCircumference()<<endl;
		out1<<"是否正方形: "<<rect2.isSquare()<<endl;		
		in1>>x1>>y1>>x2>>y2;
		rect2.resetRect(x1,y1,x2,y2);
		out1<<"重置后:\nrect2 面积: "<<rect2.getArea()<<" 周长: "<<rect2.getCircumference()<<endl;
		out1<<"是否正方形: "<<rect2.isSquare()<<endl<<endl;
	}
	in1.close();
	out1.close();
	return 0;
}
