#include<iostream>
#include<vector>
using namespace std;
template<typename T,typename Compare=std::less<T>> 
class MyQsort 
{ 
public:     
    MyQsort(T *arr, size_t size,Compare com)
        :_vec(arr,arr+size)
    {
        _com=com;
    }
    ~MyQsort(){
        _vec.clear();
        _vec.shrink_to_fit();
    }
    void quick(int left, int right);     
    int partition(int left, int right);     
    void print(); 
private:    
    vector<T> _vec; 
    typedef bool(*func)();
    Compare _com;
};

template<typename T,typename Compare>
void MyQsort<T,Compare>::quick(int left,int right){
    if(left>=right){
        //cout<<"0"<<endl;
        return;
    }
    int idex=partition(left,right);
    quick(left,idex-1);
    quick(idex+1,right);
}

template<typename T,typename Compare>
int MyQsort<T,Compare>::partition(int left,int right){
    int pivot=_vec[left];
    int i=left;
    int j=right;
    bool b=_com(1,0);
    if(!b)
    {//升序
        while(i<j){
        //自右向左
        while(i<j&&_vec[j]>=pivot){
            j--;
        }
        _vec[i]=_vec[j];
        //自左向右
        while(i<j&&_vec[i]<=pivot){
            i++;
        }
        _vec[j]=_vec[i];
        
    }}else{//降序
    while(i<j){
        //自右向左
        while(i<j&&_vec[j]<=pivot){
            j--;
        }
        _vec[i]=_vec[j];
        //自左向右
        while(i<j&&_vec[i]>=pivot){
            i++;
        }
        _vec[j]=_vec[i];
        
    }}
    //cout<<"1"<<endl;
    _vec[i]=pivot;
    return i;
}
template<typename T,typename Compare>
void MyQsort<T,Compare>::print(){
    for(auto it:_vec){
        cout<<it<<" ";
    }
    cout<<endl;
}
template<typename T>
class Com{
public:
    bool operator()(const T&lhs,const T&rhs){
        return lhs>rhs;
    }
};
int main(){
    Com<int>com;
    int arr[10]={2,34,54,76,7,4,1,4,7,65};
    MyQsort<int,Com<int>>mq(arr,10,com);
    mq.print();
    mq.quick(0,9);
    mq.print();
}
