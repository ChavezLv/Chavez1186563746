#include <iostream>
#include<string>
using namespace std;
//integer,float,science counter;
int main(){
    string str;
    bool Sign=true,sciFlag=false;
    cin>>str;
    string::iterator it=str.begin();//initial of iterator ?
    int i=0;
#if 0    //跳过空格,没必要.cin自动跳过空格
    for(;it!=str.end();++it){
        if(*it==' ') 
            i++;
        //break;
    }
    cout<<i;
#endif
    int len=str.length();
    if(str.at(i)=='-') {
        Sign=false;
        ++it;
        ++i;
    }
    else if(str.at(i)=='+') {
        ++it;
        ++i;
    }
    len=len-i;
    int count=0,sci=0;
    char ch='\0';
    int rval;//存储小数点右边位数
    for(;it!=str.end();++it){
        if(!sciFlag){
        if(*it>='0'&& *it<='9')
                count++;
        else if(*it=='.'){//有小数点的科学计数法
                if(*it==ch){   
                    cout<<"not a valid number"<<endl;
                    break;}
                ch=*it;
                count++;
                rval=count;
                count=0;//清零以便记录小数位数,同时消除int的可能性
            }
        else if(count==1&&(*it=='e'||*it=='E')){//科学计数法;
            if(ch=='e'||ch=='E'){
                    cout<<"not a valid number"<<endl;
                    break;}
            ch=*it;
            count++;
            sciFlag=true;
            continue;
            }
        else{
            cout<<"not a valid number"<<endl;
            return 0;
            }}

        if(sciFlag){//科学计数法
            if(*it=='-'||*it=='+')
            {
                count++;
                continue;}
            if(*it>='0'&& *it<='9'){
                sci++;
                if(sci==(len-count)){
                    cout<<"this is a valid number which use science count"<<endl;
                }
            }else{
                //cout<<"sci="<<sci<<endl;
                //cout<<"count="<<count<<endl;
                cout<<"error:not a valid number"<<endl;
                return 0;
            }
            
        }

        if((rval+count)==len){
            cout<<"this a decimals"<<endl;
            return 0;
        }
        if(count==len){
        cout<<"this is an integer number"<<endl;
        return 0;//整数
        }
    }
    return 0;
}
