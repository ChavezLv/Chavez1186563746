#include<iostream>
using std::ostream;
using std::cout;
using std::endl;

namespace cowString{
class Char{
public: 
    Char(){cout<<"Char"<<endl;}
    Char(const char ch);
    Char(const Char& rhs);
    ~Char(){cout<<"~Char"<<endl;}
    //Char& operator=(const Char& rhs);
    friend ostream & operator<<(ostream &os,const Char& ch);
    Char&operator=(const char ch){//只能定义为类内非静态成员函数;
            _ch=ch;
            return *this;
    }
    Char& operator=(const Char& rhs){
        if(this!=&rhs){
            _ch=rhs._ch;
        }
        return *this;
    }
private:
    char _ch='\0';//错误定义为static造成每个对象同值
};

//String
class CowString{
public:
    //构造函数版本
    CowString();
    CowString(const char * pstr);
    //CowString(const )
    //析构与拷贝构造
    ~CowString();
    CowString(const CowString& rhs);
    //运算符重载
    CowString& operator=(const CowString& rhs);
    Char operator[](size_t idex);//二元指的是索引和调用者(this)
    Char operator[](size_t idex)const;
    friend ostream &operator<<(ostream& os,const CowString& rhs);
    //引用计数操作
    int getReferenceCount();
    void ReferenceDecrease();
    void ReferenceIncrease();
    
    //其它
    void release();
    char* malloc(const char* pstr=nullptr);
    int size()const;
    void initRefenceCount();
    const char* c_str()const{return _pstr;}//返回不可修改的,类似于literal一样的值
private:
    char* _pstr;
    int _referenceCount=0;
};
}
