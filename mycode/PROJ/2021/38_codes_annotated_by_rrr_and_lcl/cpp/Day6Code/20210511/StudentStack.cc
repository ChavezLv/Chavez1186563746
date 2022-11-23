#include <string.h>
#include <iostream>

using std::cout;
using std::endl;

//只能创建栈对象
//做法：将operator new/delete设置为私有

class Student
{
public:
    Student(int id, const char *name)
    : _id(id)
    , _name(new char[strlen(name) + 1]())
    {
        cout << "Student(int, const char *)" << endl;
        strcpy(_name, name);
    }
private:
    static void *operator new(size_t sz)//私有化就是【禁止外部调用】//基本上全局私有化就是=delete的感觉了
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

public:
    void print() const
    {
        printf("this : %p\n", this);
        cout << "id : " << this->_id << endl
             << "name : " << _name << endl;
    }

    ~Student()
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
    //Q:创建栈对象需要的条件是什么？
    //A:构造函数与析构函数都不能是私有的，都必须是public
    
    Student stu(23, "xiaogang");//ok

    //Q:对象的销毁与析构函数的执行二者是不是等价的？
    //A:不等价，析构函数的执行只是对象销毁中的的一个工作步骤
    //
    //int *pInt = new int(10);
#if 0
    Student *pstu = new Student(12, "xiaohong");//error
    pstu->print();

    delete pstu;
#endif
    //如果是栈对象，对象的销毁与析构函数的执行就是一回事
    return 0;
}

