#include <iostream>
#include <iterator>
#include <vector>
#include <list>
#include <algorithm>

using std::cout;
using std::endl;
using std::back_inserter;
using std::back_insert_iterator;
using std::front_inserter;
using std::front_insert_iterator;
using std::inserter;
using std::insert_iterator;
using std::list;
using std::vector;
using std::copy;
using std::ostream_iterator;



void test()
{
    vector<int> numberVec = {1, 2, 5, 8};
    list<int> numberList = {10, 5, 7, 2, 5};//list数据写入到vector
    //方法很多: 比如遍历过程中写入, 
    //这里, 使用copy算法
    copy(numberList.begin(), numberList.end(), back_insert_iterator<vector<int>>(numberVec));
    //新遍历方式方法(逼格更高! yyds ???): 
    copy(numberVec.begin(), numberVec.end(), ostream_iterator<int>(cout, " "));
    cout << endl;

    cout << endl << endl;
    copy(numberVec.begin(), numberVec.end(), front_insert_iterator<list<int>>(numberList));
    copy(numberList.begin(), numberList.end(), ostream_iterator<int>(cout, "  "));
    cout << endl;


    //copy(arr, arr + size, back_inserter(vec));//对比arr是一个普通数组;


    cout  << endl;
    set<int> numberSet = {100, 200, 300};//除了传容器, 还要传位置
    auto it = numberSet.begin();
    copy(numberVec.begin(), numberVec.end(), \
        insert_iterator<set<int>>(numberSet, it));
    copy(numberSet.begin(), numberSet.end(), ostream_iterator<int>(cout, "  "));
    cout << endl;
}
int main(int argc, char **argv)
{
    test();
    return 0;
}

