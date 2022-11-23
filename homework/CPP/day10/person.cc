#include<iostream>
using namespace std;
/*构建一个类Person，包含字符串成员name（姓名），
 * 整型数据成员age（年龄），成员函数 display()用来输出name和age。
 * 构造函数包含两个参数，用来对name和age初始化。*/

class Person{
public:
    Person(int age,string name)
        :_age(age)
        ,_name(name){
          //  display();
             cout<<"create the item for "<<_name<<endl;
         }
    void display(){
        cout<<"name:"<<_name<<",age:"<<_age<<endl;
    }
private:
    int _age;
    string _name;
    
};

/* 构建一个类Employee由Person派生，
 * 包含department（部门），实型数据成员salary（工资）,
 * 成员函数display（）用来输出职工姓名、年龄、部门、工资，
 * 其他成员根据需要自己设定。主函数中定义3个Employee类对象，
 * 内容自己设定，将其姓名、年龄、部门、工资输出，
 * 并计算他们的平均工资。*/
class Employee
:public Person
{
public:
    Employee(int age,string name,string department,double salary)
        :Person(age,name)
        ,_department(department)
        ,_salary(salary){
          //  cout<<".."<<endl;
         }
    void display(){
        Person::display();
        cout<<"department:"<<_department<<endl<<"salary:"<<_salary<<endl;
    }
private:
    string _department;
    double _salary;
};
int main(){
//    Employee A((string)"罗翔",67,(string)"张三律师事务所",99666);
    Employee A(67,"罗翔",(string)"张三律师事务所",99666);
    A.display();
    return 0;
}
