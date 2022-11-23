#include<iostream>
#include<vector>
using namespace std;
class Int{
public:
    Int(int *arr,int size)
        :_vec(arr,arr+size)
        ,_size(size){
    }
    int getResult(){
    //利用容器自身构建hash表,时间复杂度为O(n)
    //for~while是均摊复杂度思想的体现;
        for(int i=0;i<_size;++i){
            while(_vec[i]>0&&_vec[i]<_size&&(_vec[i]!=i+1)){
                swap(_vec[i],_vec[_vec[i]-1]);
        }
    }
#if 1
        //遍历哈希容器,时间复杂度O(1)
        for(int i=0;i<_size;++i){
            if(i!=_vec[i]-1)
            {
                return i+1;
                break;
            }
        }
#endif
        return -1;
}
    void print(){
        for(int i=0;i<_size;++i){
            cout<<_vec[i]<<" ";
        }
        cout<<endl;
    }
    
private:
    vector<int>_vec;
    int _size;
};
int main(){
    int arr[5]={1,5,-2,2,3};
    Int it(arr,5);
    it.print();
    cout<<"result:"<<it.getResult()<<endl;
    it.print();
    return 0;
}
