#include<stdio.h>
#include<iostream>
using std::cout;
using std::endl;
using std::string;

class Student{
public:
    int id;
    string name;
    int age;
    void func(void);
    Student(int ID,string NAME,int AGE){
        id=ID;
        name=NAME;
        age=AGE;
    }
};
int main(){
    Student stu(10086,"王大锤",99);
    //printf("%d岁的%s的学号是%d\n",stu.age,stu.name,stu.id);
    cout<<"学号="<<stu.id<<endl;
    cout<<"名字="<<stu.name<<endl;
    cout<<"岁数="<<stu.age<<endl;
}
