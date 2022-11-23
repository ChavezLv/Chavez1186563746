#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <deque>
using std::deque;

#include <list>
using std::list;

#include <map>
using std::map;

template<typename T, typename Compare=std::less<T>>
class MyQsort
{
public:
    MyQsort(T *arr, size_t size, Compare cmp);
    void quick(int left, int right,const Compare &cmp);//传入的cmp是一个对象, const可接收右值
    int partition(int left, int right, const Compare &cmp);
    void swap(T& , T& );
    void print();
private:
    vector<T> _vec;

};

template<typename T,typename Compare>
MyQsort<T, Compare>::MyQsort(T *arr, size_t size, Compare cmp)
: _vec(arr, arr + size){

}

template<typename T,typename Compare>
void MyQsort<T, Compare>::quick(int left, int right, const Compare &cmp){
    if(left < right){
        int pivot = partition(left, right, cmp);
        quick(left, pivot - 1, cmp);
        quick(pivot + 1, right, cmp);
    }
}

template<typename T,typename Compare>
int MyQsort<T, Compare>::partition(int left, int right, const Compare &cmp){
    int i ;
    int j ;
    for(i = left, j = left; i < right; ++i){
        if(cmp(_vec[i], _vec[right])){
            swap(_vec[i], _vec[j]);
            ++j;
        }
    }
    swap(_vec[j], _vec[right]);
    return j;
}

template<typename T,typename Compare>
void MyQsort<T, Compare>::swap(T& lhs, T& rhs){
    T tmp = lhs;
    lhs = rhs;
    rhs = tmp;
}

template<typename T,typename Compare>
void MyQsort<T, Compare>::print(){
    for(auto &elem : _vec){
        cout << elem << " ";
    }
    cout << endl;
}

template<typename T>
class cmp{
public:
    //自定义比较方式
    bool operator()(const T lhs, const T & rhs) const{
        return lhs > rhs;
    } 
};

void test01(){
    std::less<int> les;
    int nums[] = { 4,21, 1, 3, 5, 7, 9, 8 };
    /* MyQsort mq(nums, 8, std::less<int>());//error */
    MyQsort<int, std::less<int>> mq(nums, 8, std::less<int>());//显式 
    mq.print();
    mq.quick(0, 7, std::less<int>());
    mq.print();
}

//自定义比较函数
void test02(){
    int nums[] = { 4,21, 1, 3, 5, 7, 9, 8 };
    MyQsort<int, cmp<int>> mq(nums, 8, cmp<int>());
    mq.print();
    mq.quick(0, 7, cmp<int>());
    mq.print();
}
int main(int argc, char* argv[])
{
    cout << "test01" << endl;
    test01();
    cout << "test02" << endl;
    test02();
    return 0;
}

