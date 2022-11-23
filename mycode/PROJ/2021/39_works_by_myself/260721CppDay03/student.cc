#include <iostream>
/* #include <string> */
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cout;
using std::endl;
using std::string;

class Student {
public:

    Student(int num, string name, int age):
        _num(num),
        _name(name),
        _age(age)
    {
        
    }

    void print(){
        cout << _num << " "
            << _name << " "
            << _age << " "
            << endl;
    }

private:
    int _num;
    string _name;
    int _age;
};
int main()
{
    Student std1(1001, "ans", 10);
    std1.print();


    return 0;
}

