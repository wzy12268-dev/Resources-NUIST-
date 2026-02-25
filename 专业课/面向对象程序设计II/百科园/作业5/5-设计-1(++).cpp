/*
矩形（Rectangle）类的成员如下： 
（1）公有成员：
Rectangle(float xx1=0.0,float yy1=0.0,float xx2=1.0,float yy2=4.0) // 构造函数，初始化矩形各数据成员，其中(xx1, yy1)为左下角坐标，(xx2, yy2)为右上角坐标
void moveTo(float newX, float newY)  // 平移操作，将左下角平移到(newX, newY)
double getCircumference () // 返回矩形的周长
bool isSquare()  // 判断是否为正方形
bool isEqual(Rectangle &r) // 判断与另一个矩形的周长是否相等
（2）私有成员：
float x1, y1, x2, y2  // 矩形左下角坐标(x1, y1)和右上角坐标(x2, y2)
double circumference  // 矩形的周长
请根据上述说明，完成Rectangle类的定义。

注意：部分源程序给出，仅允许在注释"Begin"和"End"之间填写内容，不得改动main函数和其他已有的任何内容。
试题程序：
*/
#include<iostream>
#include<fstream>
using namespace std;

/*******Begin*******/
class Rectangle{
	public:
		Rectangle(float xx1=0.0,float yy1=0.0,float xx2=1.0,float yy2=4.0){
			x1=xx1;y1=yy1;x2=xx2;y2=yy2;
			circumference=2*(x2-x1+y2-y1);
		}
		void moveTo(float newX, float newY){
			float x_offset=newX-x1,y_offset=newY-y1;
			x1=newX;
			y1=newY;
			x2+=x_offset;
			y2+=y_offset; 
		}
		double getCircumference (){
			return circumference;
		}
		bool isSquare(){
			if(x2-x1==y2-y1) return true;
			else return false;
		}  
		bool isEqual(Rectangle &r)
		{
			if(this->circumference==r.circumference) return true;
			else return false;
		}
	private:
		float x1, y1, x2, y2; // 矩形左下角坐标(x1, y1)和右上角坐标(x2, y2)
		double circumference; // 矩形的周长
}; 




/*******End*********/

int main() {
	float x1,y1,x2,y2;
	cin>>x1>>y1>>x2>>y2;
	Rectangle r1(x1,y1,x2,y2),r2(r1);
	cout<<"矩形r1: 周长"<<r1.getCircumference()<<", 是否正方形: "<<r1.isSquare()<<endl;	
	cout<<"矩形r2: 周长"<<r2.getCircumference()<<", 是否正方形: "<<r2.isSquare()<<endl;
	cout<<"周长是否相等: "<<r2.isEqual(r1)<<endl;
	cin>>x1>>y1;
	r2.moveTo(x1,y1);
	cout<<"平移后:\n矩形r1: 周长"<<r1.getCircumference()<<", 是否正方形: "<<r1.isSquare()<<endl;	
	cout<<"矩形r2: 周长"<<r2.getCircumference()<<", 是否正方形: "<<r2.isSquare()<<endl;
	cout<<"周长是否相等: "<<r2.isEqual(r1)<<endl;
	
	ifstream in1("4.1.1.3_4-2_in.dat");
	ofstream out1("4.1.1.3_4-2_out.dat");
	while(in1>>x1>>y1>>x2>>y2)
	{
		Rectangle r1(x1,y1,x2,y2),r2(r1);
		out1<<"矩形r1: 周长"<<r1.getCircumference()<<", 是否正方形: "<<r1.isSquare()<<endl;	
		out1<<"矩形r2: 周长"<<r2.getCircumference()<<", 是否正方形: "<<r2.isSquare()<<endl;
		out1<<"周长是否相等: "<<r2.isEqual(r1)<<endl;
		in1>>x1>>y1;
		r2.moveTo(x1,y1);
		out1<<"平移后:\n矩形r1: 周长"<<r1.getCircumference()<<", 是否正方形: "<<r1.isSquare()<<endl;	
		out1<<"矩形r2: 周长"<<r2.getCircumference()<<", 是否正方形: "<<r2.isSquare()<<endl;
		out1<<"周长是否相等: "<<r2.isEqual(r1)<<endl<<endl;		
	}
	in1.close();
	out1.close();
	return 0;
}
