#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

class Person {
public:
    Person(string name = 0, int age = 0)
    : _name(name)
    , _age(age){

    }
    void display(){
        cout << "Person: name(" << getName() << "), age(" << getAge() << ")" << endl;
    }
protected:
    string getName(){
        return _name;

    }              
    int getAge(){  
        return _age;

    }              

private:           
    string _name;  
    int _age;      


};                 

class Employee     
: public Person{   
public:
    Employee(string name = 0, int age = 0, string department = 0, double salary = 0)
    : Person(name, age)
    , _department(department)
    , _salary(salary){
        
    }
    void display(){
        cout << "Person: name(" << getName() << "), age(" << getAge() << "), department(" << \
            _department << "), salary(" << _salary << ")" << endl; 
    }               

    double getSalary(){
        return _salary;
    }

private:            
    string _department;
    double _salary; 
};                  
int main()          
{                   
    /* Employee staff1("Kim", 19, "HR Dept",6000); */
    /* Employee staff2("Candy", 29, "PR department",16000); */
    /* Employee staff3("Zed", 39, "R&D",26000); */
    Employee staff[3] = {{"Kim", 19, "HR Dept",6000}, {"Candy", 29, "PR department",16000},\
            {"Zed", 39, "R&D",26000}};

    staff[0].display();
    staff[1].display();
    staff[2].display();
    
    cout << "average salary:" << (staff[0].getSalary() + staff[1].getSalary()\
                                  + staff[2].getSalary()) / 3 << endl;
    
    return 0;
}

