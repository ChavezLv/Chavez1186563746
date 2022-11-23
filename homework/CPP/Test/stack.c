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
	bool empty(StackData s);	//判断栈是否为空
	bool full(StackData s);	//判断栈是否已满
	void push(StackData* s,int val); //元素入栈
	void pop(StackData* s);     //元素出栈
	int  top(StackData s);		//获取栈顶元素
    StackData s;
};
int main(){
    //StackData s_d;
    StackOperate s_op;

}


int StackOperate::top(StackData s){
    if(!empty(s)){
        return s.data[s.ptr];
    }
    return -1;
}

bool StackOperate::empty(StackData s){
    if(!s.size) return true;
}
bool StackOperate::full(StackData s){
    if(s.size) return true;
}
void StackOperate::push(StackData* s,int val){
    if(full(*s))
    {
        cout<<"the stack is full!"<<endl;
        return;
    }else
    {
        s->data[++s->ptr]=val;
        s->size++;
    }}

void StackOperate::pop(StackData* s){
    if(empty(*s))
    {
        cout<<"the stack is full!"<<endl;
        return;
    }else
    {
        --s->ptr;
        --s->size;
    }
}
