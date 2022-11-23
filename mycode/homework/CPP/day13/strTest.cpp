#include"string.cpp"
using namespace std;
void test1(){
    String str1("hello");
    String str2=str1;
    cout<<"str1:"<<str1<<"; str2:"<<str2<<endl;
    cin>>str1;
    cout<<"istream:"<<str1<<endl;
    //char* str3="hello";
    //cout<<strlen(str3)<<endl;
}
void test2(){
    String str1("hello");
    String str2;
    str2=str1;
    cout<<"str1:"<<str1<<"; str2:"<<str2<<endl;
    str1="world";
    cout<<"str1:"<<str1<<endl;
}
void test3(){
    String str1("hello");
    String str2;
    str1+=" world";
    str2+=str1;
    cout<<"str1:"<<str1<<"; str2:"<<str2<<endl;
}
void test4(){
    String str1("hello");
    String str2;
    str1+=" world";
    str2+=str1;
    str2[4]='M';
    cout<<"str1:"<<str1<<"; str2:"<<str2<<endl;
    const char ch=str2[4];//无论加不加const,都是调用非const版本的[]
    cout<<"ch="<<ch<<endl;
    cout<<"str2.size="<<str2.size()<<endl;//不包括'\n'
    const char* pstr=new char();
    pstr=str2.c_str();
    cout<<"pstr:"<<pstr<<endl;
}
void test5(){
    String str1("hello");
    String str2;
    str1+=" world";
    str2+=str1;
    String str3=str1+str2;
    String str4,str5;
    str4=str1+"china";
    str5="china"+str3;
    cout<<"str1:"<<str1<<"; str2:"<<str2<<endl;
    cout<<"str3:"<<str3<<"; str4:"<<str4<<endl;
    cout<<"str5:"<<str5<<endl;
}
int main(){
    test5();
    return 0;
}
