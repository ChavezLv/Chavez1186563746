#include<string>
#include<iostream>
using namespace std;
class Atoi{
public:
    int myAtoi(string str);
private:
    int res=0;
    bool flag=false;
    bool isEmpty=true;
    int it=0;
    int num;
};
int Atoi::myAtoi(string str){
    //if(isEmpty) return 0;
    while(str[it]==' ')it++;
    if(str[it]=='+')it++;
    else if(str[it]=='-'){
        flag=true;
        it++;}

    while(str[it]>='0' && str[it]<='9'){
        num=str[it]-'0';
        if(!flag){  //正数
            if(res>=INT32_MAX/10) return INT32_MAX;
            else res=res*10+num;
        }else{      //负数
            if(res<=INT32_MIN/10) return INT32_MIN;
            else res=res*10-num;
        }
        it++;
    }
    return res;
}
int main(){
    Atoi a;
    int aa=a.myAtoi("999999999999999");
    printf("%d\n",aa);
}
