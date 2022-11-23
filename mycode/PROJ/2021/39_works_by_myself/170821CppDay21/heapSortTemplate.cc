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

#include <algorithm>
#include <functional>
//for_each

template<typename T = int, typename Compare = std::less<T> >
class HeapSort {
public:
    HeapSort(T *arr, size_t size, Compare com){
        copy(arr, arr + size, back_inserter(vec));
        /* uninitialized_copy(arr, arr + size, back_inserter(vec)); //error,value type is destructible */
    }

    void adjustMaxHeap(int pos, size_t size, Compare &com);
    void sort(Compare &com);

    void print(){
        for_each(vec.begin(), vec.end(), [] (const int &elem){
                 cout << elem << " ";
                 });
    }

    inline
    void swap(T &lhs, T &rhs){
        int tmp = lhs;
        lhs = rhs;
        rhs = tmp;
    }

private:
    vector<T> vec;
};

template<typename T, typename Compare>
void HeapSort<T, Compare>::adjustMaxHeap(int pos, size_t size, Compare &com){
    int dad = pos;
    int son = 2 * dad + 1;
    while(son < (int)size){
        if(son + 1 < size && com(vec[son], vec[son + 1])){
            ++ son;
        }
        
        if(com(vec[dad], vec[son])){
            swap(vec[son], vec[dad]);
            dad = son;
            son = 2* dad + 1;
        }else{
            break;
        }
    }

}

template<typename T, typename Compare >
void HeapSort<T, Compare>::sort(Compare &com){
    for(int posDad = vec.size() / 2 - 1; posDad >= 0; --posDad){
        adjustMaxHeap(posDad, vec.size(), com);
    }
    swap(vec.front(), vec.back());//return reference
    for(int adjust_size = vec.size() - 1; adjust_size >= 2; --adjust_size){
        adjustMaxHeap(0, adjust_size, com);
        swap(vec[0], vec[adjust_size - 1]);
    }

}



void test01(){
    int arr[10] = { 3, 4, 4, 5, 6, 8, 2, 9, 1 };
    HeapSort<int> vec(arr, 10, std::less<int>());
    std::less<int> les;

    std::function<void()> fsort = bind(&HeapSort<int>::sort, &vec, les);
    std::function<void()> fprint = bind(&HeapSort<int>::print, &vec);
    fsort();
    fprint();

    /* vec.sort(les); */
    /* vec.print(); */
}
int main(int argc, char* argv[])
{

    test01();
    return 0;
}

