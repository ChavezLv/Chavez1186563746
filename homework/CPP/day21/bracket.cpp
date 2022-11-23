#include<iostream>
#include<stack>
using std::cout;
using std::endl;
using std::string;
using std::stack;
int main(){
    stack<char>bracket;
    bool flag=true;
    int rcount=0,lcount=0;//多判断一下左右括号是否相等
    string str;
    std::cin>>str;
    for(int i=0;i<(int)str.length();++i){
        char ch=str[i];
        if(ch=='('||ch=='['||ch=='{'){
            lcount++;
            if(ch=='(') bracket.push(ch+1);
            else bracket.push(ch+2);
            continue;
        }
        if(ch==')'||ch=='}'||ch==']'){
            rcount++;
            if(!bracket.empty()){
                if(ch==bracket.top()){
                    bracket.pop();
                    continue;
                }else{
                    flag=false;
                    break;
                }
            }else{
                flag=false;
                break;
            }
        }
    }
    if(flag&&rcount==lcount){
        cout<<"true"<<endl;
    }else{
        cout<<"false"<<endl;
    }
    return 0;
}
