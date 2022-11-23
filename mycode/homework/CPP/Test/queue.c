#include<iostream>
#define SIZE 10
using namespace std;
struct QueueData{
    int Front=0;
    int Rear=0;
    int size=0;
    int data[SIZE];
};

/*静态成员变量属于所有类，存放在静态区，
 * 但是非静态成员变量的存放和类实例的空间有关，
 * 类如果实例化在堆上，也就是New开辟的空间
 * ，这个成员变量就是存放在堆上，
 * 如果类的实例化在栈上，这个成员变量就是在栈上。*/
class QueueOperate{
public:
	bool empty();	//判断队是否为空
	bool full();	//判断队是否已满
	void push(int val); //元素入队
	void pop();     //元素出栈
	int  front();//获取队头元素
    int back();//获取队尾元素
    QueueData s;};
int main(){
    //StackData s_d;
    QueueOperate qop;
    for(int i=1;i<11;++i)
        qop.push(i);
    //printf("size=%d",qop.s.size);
    for(int i=0;i<10;++i){
        if(!qop.empty())
            printf("front=%d,back=%d\n",qop.front(),qop.back());
        qop.pop();
}
    cout<<endl;
}


int QueueOperate::front(){
    if(!empty()){
        return s.data[s.Front];
    }
    return -1;}
