#include<iostream>
#include<vector>
using namespace std;

template<typename T,int MAXSIZE>
class HeapSort{
public:
    HeapSort(int *arr)
        :_vec(arr,arr+MAXSIZE),_len(MAXSIZE){}
    void build_BigHeap(){
        //从后往前建立大顶堆
        for(int i=((_len-2)>>1);i>=0;--i){
            heap(i,_len);
        }
    }
    void heap(int idx,int len){
        //判断条件生效于建立大顶堆之后的排序
        while(idx<len){
        int lchild=2*idx+1;
        int rchild=2*idx+2;
        int maxIdex=idx;
            //lchild必须在无序区内
            if(lchild<len&&_vec[lchild]>_vec[maxIdex]){
                maxIdex=lchild;
            }
            if(rchild<len&&_vec[rchild]>_vec[maxIdex]){
                maxIdex=rchild;
            }
            if(maxIdex==idx){return;}
            swap(_vec[idx],_vec[maxIdex]);
            idx=maxIdex;
        }

    }
    void heapSort(){
        build_BigHeap();
        //交换堆顶元素和无序区最后元素;
        while(_len>1){
            swap(_vec[0],_vec[_len-1]);
            --_len;
            //自顶向下重新构建大顶堆
            heap(0,_len);
        }
    }
    void print(){
        for(auto e:_vec){
            cout<<e<<" ";
        }
        cout <<endl;
    }
private:
    vector<T>_vec;
    int _len;
};
int main(){
    int arr[5]={1,5,3,7,2};
    HeapSort<int,5> hs(arr);
    hs.print();
    hs.heapSort();
    hs.print();
    return 0;
}
