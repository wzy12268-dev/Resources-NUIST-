/*
点（Point）类成员如下： 
（1）公有成员：
Point(float xx, float yy)  // 构造函数，初始化点的x, y坐标
float getX() const  // 返回横坐标x
float getY() const  // 返回纵坐标y
void setX(float newX)  // 重设横坐标为newX
void setY(float newY)  // 重设纵坐标为newY
（2）私有成员：
float x, y  // 点的横坐标，纵坐标
由Point类公有派生出圆（Circle）类，基类Point的x, y成员作为圆心的坐标，并新增如下成员：
（1）公有成员：
Circle(float x=0.0, float y=0.0, float r=1.0)  // 构造函数，其中(x,y)为圆心位置，r为圆的半径
void moveTo(float newX, float newY)  // 平移操作，将圆心移动到newX, newY
float getRadius() const  // 返回圆的半径
double getCircumference() const  // 计算并返回圆的周长，π=3.14159
double getArea() const  // 计算并返回圆的面积，π=3.14159
double dist(const Circle &c) const  // 计算并返回到另一个圆的距离（圆心之间的距离）
bool isEqual(const Circle &c) const  // 判断与另一个圆是否大小相等
（2）私有成员：
float radius  // 圆的半径 

请根据上述说明，完成Point，Circle两个类的定义。
注意：部分源程序已给出，仅允许在注释“Begin”和“End”之间填写内容，不得改动其他已有的任何内容。

测试样例：
输入：
0 0 1
3 4
输出：
初始: 
c1：圆心: (0, 0), 半径: 1, 周长: 6.28318, 面积: 3.14159
c2：圆心: (0, 0), 半径: 1, 周长: 6.28318, 面积: 3.14159
c1与c2圆心之间距离：0
c1与c2大小是否相等：1

平移后: 
c2：圆心: (3, 4), 半径: 1, 周长: 6.28318, 面积: 3.14159
c1与c2圆心之间距离：5
c1与c2大小是否相等：1

试题程序：
*/
#include<iostream>
#include<fstream>
#include<cmath>
using namespace std;
const double PI=3.14159;

/*******Begin*******/
class Point{
	public:
		Point(float xx, float yy):x(xx),y(yy){}
		float getX() const{return this->x;}  
		float getY() const{return this->y;}
		void setX(float newX){
			this->x=newX;
		}
		void setY(float newY){
			this->y=newY;
		}
	private:
		float x,y;
}; 
class Circle:public Point{
	public:
		Circle(float x=0.0, float y=0.0, float r=1.0):Point(x,y){
			this->radius=r;
		}
		void moveTo(float newX, float newY){
			setX(newX);
			setY(newY);	
		}
		float getRadius() const
		{
			return this->radius;}
		double getCircumference() const{
			return 2*PI*radius;}
		double getArea() const{
			return PI*radius*radius;}
		double dist(const Circle &c) const{
			double x=this->getX()-c.getX();
			double y=this->getY()-c.getY();
			return sqrt(x*x+y*y);
		}
		bool isEqual(const Circle &c) const
		{
			if(this->radius==c.radius) return true;
			else return false;
		}
	private:
		float radius;
};


/*******End*********/

int main() {
	float x,y,r;
	cin>>x>>y>>r;
	Circle c1(x,y,r),c2;
	cout<<"初始: "<<endl;
	cout<<"c1：圆心: ("<<c1.getX()<<", "<<c1.getY()<<"), "<<"半径: "<<c1.getRadius()<<", 周长: "<<c1.getCircumference()<<", 面积: "<<c1.getArea()<<endl;
	cout<<"c2：圆心: ("<<c2.getX()<<", "<<c2.getY()<<"), "<<"半径: "<<c2.getRadius()<<", 周长: "<<c2.getCircumference()<<", 面积: "<<c2.getArea()<<endl;
	cout<<"c1与c2圆心之间距离："<<c1.dist(c2)<<endl;	
	cout<<"c1与c2大小是否相等："<<c1.isEqual(c2)<<endl<<endl;
	
	cin>>x>>y;
	c2.moveTo(x,y);
	cout<<"平移后: "<<endl;
	cout<<"c2：圆心: ("<<c2.getX()<<", "<<c2.getY()<<"), "<<"半径: "<<c2.getRadius()<<", 周长: "<<c2.getCircumference()<<", 面积: "<<c2.getArea()<<endl;
	cout<<"c1与c2圆心之间距离："<<c1.dist(c2)<<endl;	
	cout<<"c1与c2大小是否相等："<<c1.isEqual(c2)<<endl<<endl;

	ifstream in1("7.1.2_1-s2_in.dat");
	ofstream out1("7.1.2_1-s2_out.dat");
	while(in1>>x>>y>>r) {
		Circle c1(x,y,r),c2;
		out1<<"初始: "<<endl;
		out1<<"c1：圆心: ("<<c1.getX()<<", "<<c1.getY()<<"), "<<"半径: "<<c1.getRadius()<<", 周长: "<<c1.getCircumference()<<", 面积: "<<c1.getArea()<<endl;
		out1<<"c2：圆心: ("<<c2.getX()<<", "<<c2.getY()<<"), "<<"半径: "<<c2.getRadius()<<", 周长: "<<c2.getCircumference()<<", 面积: "<<c2.getArea()<<endl;
		out1<<"c1与c2圆心之间距离："<<c1.dist(c2)<<endl;	
		out1<<"c1与c2大小是否相等："<<c1.isEqual(c2)<<endl<<endl;
		
		in1>>x>>y;
		c2.moveTo(x,y);
		out1<<"平移后: "<<endl;
		out1<<"c2：圆心: ("<<c2.getX()<<", "<<c2.getY()<<"), "<<"半径: "<<c2.getRadius()<<", 周长: "<<c2.getCircumference()<<", 面积: "<<c2.getArea()<<endl;
		out1<<"c1与c2圆心之间距离："<<c1.dist(c2)<<endl;	
		out1<<"c1与c2大小是否相等："<<c1.isEqual(c2)<<endl<<endl;
	}
	in1.close();
	out1.close();
	return 0;
}
