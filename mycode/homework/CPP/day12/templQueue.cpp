#include<iostream>
#define M 8
using namespace std;
template<typename Q,int MAXSIZE=8>
class Queue{
public:
    Queue()
        :_data(new Q[MAXSIZE]()),_front(0),_rear(0){}

    ~Queue(){
        if(_data){
            delete [] _data;
            _data=nullptr;
        }
    }

    bool isFull()const;
    bool isEmpty()const;
    void pushQueue(const Q& val);
    Q popQueue();
    Q peak();
    Q rear(){
        int tmp=0;
        if(_rear==0)tmp=MAXSIZE-1;
        else tmp=_rear-1;
        return _data[tmp];
    }
private:
    Q* _data;
    int  _front;
    int _rear;
    int _size=0;
    //Q _rear;
};

template<typename Q,int MAXSIZE>
bool Queue<Q,MAXSIZE>::isFull()const{
    if(_size==MAXSIZE)return true;
    else return false;
}
template<typename Q,int MAXSIZE>
bool Queue<Q,MAXSIZE>::isEmpty()const{
    if(_size==0)return true;
    else return false;
}

template<typename Q,int MAXSIZE>
void Queue<Q,MAXSIZE>::pushQueue(const Q&val){
    if(isFull()){
        cout<<"the queue is full"<<endl;
    }else{
        ++_size;
        _data[_rear]=val;
        _rear=(_rear+1)%MAXSIZE;
    }
}
template<typename Q,int MAXSIZE>
Q Queue<Q,MAXSIZE>::popQueue(){
    if(isEmpty()){
        cout<<"the queue is empty"<<endl;
    }else{
        --_size;
        Q tmp=_data[_front];
        _front=(_front+1)%MAXSIZE;
        return tmp;
    }
}
template<typename Q,int MAXSIZE>
Q Queue<Q,MAXSIZE>::peak(){
    return _data[_front];
}
/*test*/
int main(){
    Queue<double,M>que;
    for(int i=0;i<M;++i){
        double tmp=(double)i*3.14;
        que.pushQueue(tmp);
        cout<<"rear:"<<que.rear()<<endl;
    }
    cout<<endl;
    for(int i=0;i<M;++i){
        cout<<"peak:"<<que.popQueue()<<endl;
    }
    return 0;
}
