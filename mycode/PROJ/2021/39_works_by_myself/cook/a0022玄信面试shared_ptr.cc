#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>
#include <thread>
using namespace std;

int a = 100;
std::shared_ptr<int> p1(new int(1000));

void JudgeIsThreadSur(std::shared_ptr<int>& p2)
{
	p2.swap(p1);
	p1.swap(p2);
}
void func()
{
	std::shared_ptr<int> p2(new int(2000));
	for (int i = 0; i < 1000000; ++i)
	{
		JudgeIsThreadSur(p2);
		if (*p2.get() != 2000)
		{
			std::cout << "线程错误\n";
			break;
		}
	}
}
class SharedPtr
{
public:
        SharedPtr(T*ptr=NULL)
               :_ptr(ptr)
               , _pcount(new int(1))
        {}
        SharedPtr(const SharedPtr&s)
               :_ptr(s._ptr)
               , _pcount(s._pcount)
        {
               *(_pcount)++;
        }
        SharedPtr<T>&operator=(const SharedPtr&s)
        {
               if (this!= &s)
               {
                       if (--(*(this->_pcount)) == 0)
                       {
                              delete this->_ptr;
                              delete this->_pcount;
                       }
                       _ptr = s._ptr;
                       _pcount = s._pcount;
                       *(_pcount)++;
               }
               return *this;
        }
        T&operator*()
        {
               return *(this->_ptr);
        }
        T*operator->()
        {
               return this->_ptr;
        }
        ~SharedPtr()
        {
               --(*(this->_pcount));
               if (this->_pcount == 0)
               {
                       delete _ptr;
                       _ptr = NULL;
                       delete _pcount;
                       _pcount = NULL;
               }
        }
private:
        T*_ptr;
        int *_pcount;//指向引用计数的指针
};
/* class SharedPtr{ */
/* public: */
            
/* private: */
    
/* }; */

void test01(){
	for (int i = 0; i < 2; ++i)
	{
		std::thread t(func);
		t.detach();
	}
	while (1);
}

void test02(){

}

int main()
{
    /* test01(); */
    test02();
	return 0;
}
