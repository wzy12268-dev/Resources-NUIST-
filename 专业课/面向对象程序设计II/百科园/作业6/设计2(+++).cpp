/*
课程（Course）类主要用于存储某门课程的基础信息和考试成绩，并统计该课程的最高分、最低分、平均分和不及格的学生人数。
Course类的成员声明已给出，请参照注释，完成Course类的定义。

注意：部分源程序给出，仅允许在注释"Begin"和"End"之间填写内容，不得改动main函数和其他已有的任何内容。
试题程序：
*/
#include<iostream>
#include<fstream>
#include<string>
using namespace std;

class Course {
		int number;    // 课程编号 
		string name;   // 课程名称 	
		int credit;    // 课程学分
		int n;         // 选修课程的学生数量
		float *score;  // 指向保存学生成绩的数组 
		int max,min;   // 课程的最高分、最低分
		float average; // 课程的平均分
		int count;     // 不及格（<60）的学生数量
	public:
		Course(int n);   // 构造函数，动态创建一个长度为n的数组，返回值赋给score，n为学生数量 
		Course(const Course &c); // 实现对象的深复制
		~Course();       // 析构函数，删除成绩数组
		void input();    // 依次输入课程编号、课程名称、课程学分，以及n名学生的成绩 
		void process();  // 计算课程的最高分、最低分、平均分，以及不及格的学生数量
		void print() {   // 输出课程编号、课程名称、课程学分、最高分、最低分、平均分以及不及格学生数
			cout<<"课程编号："<<number<<endl;
			cout<<"课程名称："<<name<<" 课程学分："<<credit<<endl;
			cout<<"最高分："<<max<<" 最低分："<<min<<" 平均分："<<average<<" 不及格数："<<count<<endl<<endl;
		}  
};  

/*******Begin*******/
Course::Course(int n){
	this->n=n;
	score=new float[n];
} 
Course::Course(const Course &c){
	number=c.number,name=c.name,credit=c.credit,n=c.n,
	max=c.max,min=c.min,count=c.count,average=c.average;
	score=new float[n];
	for(int i=0;i<n;i++){
		score[i]=c.score[i];
	}
}
Course::~Course(){delete[] score;}
void Course::input(){
	cin>>number>>name>>credit;
	for(int i=0;i<n;i++){
		cin>>score[i];
	}
}
void Course::process(){
	max=min=score[0],count=0,average=0;
	for(int i=0;i<n;i++)
	{
		if(max<score[i]) max=score[i];
		if(min>score[i]) min=score[i];
		if(score[i]<60) count++;
		average+=score[i];
	}
	average/=n;
}




/*******End*********/

int main() {
	int n;
	cin>>n;
	Course c1(n);
	c1.input();c1.process();c1.print();
	Course c2(c1);c2.print();

	ifstream in1("6.6.2_3_in.dat");
	ofstream out1("6.6.2_3_out.dat");
	streambuf *cinbackup;
	streambuf *coutbackup;
	cinbackup=cin.rdbuf(in1.rdbuf());
	coutbackup=cout.rdbuf(out1.rdbuf());	
	while(cin>>n) {
		Course c1(n);
		c1.input();c1.process();c1.print();
		Course c2(c1);c2.print();
	}	
	cin.rdbuf(cinbackup);
	cout.rdbuf(coutbackup);
	in1.close();
	out1.close();
	return 0;
}
