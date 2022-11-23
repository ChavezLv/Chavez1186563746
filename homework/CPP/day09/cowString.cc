#include<string.h>
#include"cowString.hpp"
namespace cowString{
    Char::Char(const char ch){
        _ch=ch;
    }
    Char::Char(const Char& rhs){
        _ch=rhs._ch;
    }
ostream& operator<<(ostream &os,const Char& ch){
    os<<ch._ch;
    return os;
}
/*
 * 自定义string类型
 */
#if 1
//构造函数与析构,拷贝构造函数
CowString::CowString(){
    malloc();
}
CowString::CowString(const char * pstr)
: _pstr(malloc(pstr))
{
	cout << "CowString(const char *)" << endl;
	strcpy(_pstr, pstr);
	initRefenceCount();
}
CowString::CowString(const CowString& rhs){
    malloc(rhs._pstr);
    this->_pstr=rhs._pstr;
    ReferenceIncrease();
}
CowString::~CowString(){
    if(getReferenceCount()==0){
        delete []_pstr;
    }else{
        ReferenceDecrease();
    }
}

//重载运算符[],<<,=
CowString& CowString::operator=(const CowString &rhs){
    if(this!=&rhs){//判断条件不可以*this!=rhs
        /*if(this->_pstr&&getReferenceCount(_pstr)==0){
            delete []_pstr;
        }*/
        //malloc(rhs._pstr);//没必要
        //注释部分是为了回收左操作数的空间实现override;封装在release()
        release();
        this->_pstr=rhs._pstr;
        ReferenceIncrease();
    }
    return *this;
}
//写操作的下表访问运算符
Char CowString::operator[](size_t idex){
    if(static_cast<int>(idex)<size()){
        if(getReferenceCount()>1){
            char* ptmp=malloc(_pstr);
            strcpy(ptmp,_pstr);
            ReferenceDecrease();
            ptmp=_pstr;
            initRefenceCount();
        }else{
            Char temp;
            temp=_pstr[idex];
            return temp;//directly modify the literal;
        }
    }else{
        static char ch='\0';
        return ch;
    }
    return this->_pstr[idex];
}
//读操作的下标访问运算符
Char CowString::operator[](size_t idex)const{
    if(static_cast<int>(idex)<size()){
        Char temp;
        temp=_pstr[idex];
        return temp;//directly modify the literal;
    }else{
        static char ch='\0';
        return ch;
    }
}
//重载输出流运算符
ostream&operator<<(ostream& os,const CowString& rhs){
    os<<rhs._pstr;
    return os;
}


//引用计数相关操作
void CowString::initRefenceCount(){
    *reinterpret_cast<int *>(_pstr-_referenceCount)=1;
}//引用计数设为一
int CowString::getReferenceCount(){
    return *reinterpret_cast<int *>(_pstr-_referenceCount);
}
void CowString::ReferenceDecrease(){
    --*reinterpret_cast<int *>(_pstr-_referenceCount);
}
void CowString::ReferenceIncrease(){
    ++*reinterpret_cast<int *>(_pstr-_referenceCount);
}

//其它接口
void CowString::release(){
    ReferenceDecrease();
    if(getReferenceCount()==0){
        delete[](_pstr-_referenceCount);
        _pstr=nullptr;
    }
    
}
int CowString::size()const{
    return *reinterpret_cast<int*>(_pstr-_referenceCount);
}
char* CowString::malloc(const char* pstr){//深拷贝时使用
    if(_pstr){//_pstr不为空时
        _pstr=new char[(sizeof(pstr)+1              \
                +_referenceCount)]()+_referenceCount;
    }else{//在默认构造函数中使用,先申请一个字节并指向;(指针占四个字节)
        _pstr=new char[(1+_referenceCount)]()+_referenceCount;
        }
    return _pstr;
    }
#endif
}
