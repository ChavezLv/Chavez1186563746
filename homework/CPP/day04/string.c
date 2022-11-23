#include <iostream>
using namespace std;

void assign(const char* pstr,char*& _pstr,int &length){
    //_pstr必须使用引用
        for(;pstr[length];++length) ;
        _pstr=new char[length+1];
        for(int i=0;i<length;++i)
            _pstr[i]=pstr[i];
}

class String
{
public:
    String(){
        cout<<"default constructor"<<endl;
    }
	String(const char *pstr);
    String(const String &rhs);
    size_t length() const;
    void print(){
        cout<<_pstr<<endl;//<<"length="<<length;
    }

    ~String(){
        delete []_pstr;
        cout<<"the heap space of _pstr has free!"<<endl;
    }
	String &operator=(const String &rhs);
    String &operator=(const char *rhs);
#if 0
    const char * c_str() const;//?
#endif
private:
    int id;
	char * _pstr=new char[1];//先开辟一个空间,否则释放时调用delete将发生错误;
};
String & String::operator=(const String &rhs){
    if(this==&rhs){
        return *this;
    }else{
        delete []_pstr;//override
        int length=0;
        assign(rhs._pstr,_pstr,length);
        cout<<"assin for = with type of class String"<<endl;
        return *this;
    }
}

String & String::operator=(const char *rhs){
        delete []_pstr;//override
        int length=0;
        assign(rhs,_pstr,length);
        cout<<"assin for = with type of charater literal"<<endl;
        return *this;
}
size_t String::length() const{
    //初始化的时候获取过一次长度;
    size_t len=0;
    for(;_pstr[len];++len) ;
    return len;
}
String::String(const char* pstr){
    int length=0;
    if(pstr==NULL){
        cout<<"invalid character literal"<<endl;
    }else{
        assign(pstr,_pstr,length);
    cout<<"user constructor print:"<<_pstr<<endl;
    }
}

String::String(const String &rhs){//拷贝构造函数
    int len;
    assign(rhs._pstr,_pstr,len);//非const可以传给const,反之不行;
    cout<<"copy print:"<<_pstr<<endl;
}
int main()
{
#if 0
    String str1("hello world!");
    cout<<"str1: length="<<str1.length()<<endl;
    String str2(str1);
	str1.print();		
	str2.print();	
	String str3= "4:Hello,world";
	String str4;
    str4= str3;
	str3.print();		
	str4.print();	
#endif
    String str1;
	str1.print();


	String str2 = "Hello,world";
	String str3("wangdao");

	str2.print();
	str3.print();

	String str4 = str3;
	str4.print();

	str4 = str2;
	str4.print();
    
	String str5;
    str5="5:Hello,world";
    str5.print();
	return 0;
}

