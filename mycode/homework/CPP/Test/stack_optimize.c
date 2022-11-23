#include<iostream>
#define SIZE 1024
using namespace std;
struct StackData{
    int ptr=0;
    int size=0;
    int data[SIZE];
};
class StackOperate{
public:
	bool empty();	//判断栈是否为空
	bool full();	//判断栈是否已满
	void push(int val); //元素入栈
	void pop();     //元素出栈
	int  top();		//获取栈顶元素
    StackData s;
};
int main(){
    //StackData s_d;
    StackOperate sop;
    for(int i=1;i<11;++i)
        sop.push(i);
    for(int i=0;i<10;++i){
        if(sop.top()!=-1)
            printf("%d,",sop.top());
        sop.pop();
}
    cout<<endl;
}


int StackOperate::top(){
    if(!empty()){
        return s.data[s.ptr];
    }
    return -1;
}

bool StackOperate::empty(){
    if(0==s.size) return true;
    else{return false;}
}
bool StackOperate::full(){
    if(s.size==SIZE) return true;
    else{return false;}
}
void StackOperate::push(int val){
    if(full())
    {
        cout<<"the stack is full!"<<endl;
        return;
    }else
    {
        s.data[++s.ptr]=val;
        s.size++;
    }}

void StackOperate::pop(){
    if(empty())
    {
        cout<<"the stack is full!"<<endl;
        return;
    }else
    {
        --s.ptr;
        --s.size;
    }
}
