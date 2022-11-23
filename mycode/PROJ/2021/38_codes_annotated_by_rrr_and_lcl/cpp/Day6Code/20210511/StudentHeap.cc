#include <string.h>
#include <iostream>

using std::cout;
using std::endl;

class Student
{
public:
    Student(int id, const char *name)
    : _id(id)
    , _name(new char[strlen(name) + 1]())//构造函数好像都写在里面
    {
        cout << "Student(int, const char *)" << endl;
        strcpy(_name, name);
    }

    static void *operator new(size_t sz)
    {
        /* printf("this : %p\n", this);//error */
        cout << "void *operator new(size_t)" << endl;
        void *ptr = malloc(sz);

        return ptr;
    }

    static void operator delete(void *pret)
    {
        /* printf("this : %p\n", this);//error */
        cout << "void operator delete(void *)" << endl;
        free(pret);
    }

    void print() const
    {
        printf("this : %p\n", this);
        cout << "id : " << this->_id << endl
             << "name : " << _name << endl;
    }

    void destroy()
    {
        /* this->~Student();//error */
        delete this;//delete对象本身
    }
private://其实构造和析构都可以放进来会比较好，但是单放析构系统已经不允许了
    ~Student()//效果同=delete
    {
        cout << "~Student()" << endl;
        if(_name)
        {
            delete [] _name;
            _name = nullptr;
        }
    }
private:
    int _id;
    char *_name;
};
int main(int argc, char **argv)
{
    /* Student stu(23, "xiaogang");//error */

    //Q:对象的销毁与析构函数的执行二者是不是等价的？
    //A:不等价，析构函数的执行只是对象销毁中的的一个工作步骤
    //
    //int *pInt = new int(10);
    Student *pstu = new Student(12, "xiaohong");//堆对象,ok
    pstu->print();

    pstu->destroy();
    /* delete pstu;//error */

    //如果是栈对象，对象的销毁与析构函数的执行就是一回事
    return 0;
}

