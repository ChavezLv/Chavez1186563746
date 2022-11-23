#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

template<typename Tp, typename Compare = std::less<Tp> >
class Solution{
public:
    void quickSort(vector<Tp> &vec, const int &left, const int& right){
        if(left < right){
            int behinBoundary = partition(vec, left, right);
            quickSort(vec, left, behinBoundary - 1);
            quickSort(vec, behinBoundary + 1, right);
        }
    }

    int partition(vector<Tp> &vec, const size_t & left, const size_t& right){
        size_t behinBoundary = left;
        for(size_t startIndex = left; startIndex < right; ++ startIndex){
            if(_cmp(vec[startIndex], vec[right])){
                swap(vec[startIndex], vec[behinBoundary]);
                ++behinBoundary;
            }
        }
        swap(vec[behinBoundary], vec[right]);
        return behinBoundary;
    }
    /* Solution() */
    /* : _cmp(std::less<Tp>){ */

    /* } */
private:
    Compare _cmp;//这个写为&就要在构造函数初始化一下
    function<void(Tp&, Tp&)> swap = [](Tp & lhs, Tp &rhs){
        Tp tmp = lhs;
        lhs = rhs;
        rhs = tmp;
    };
    /* static function<void(Tp&, Tp&)> swap; */
};
/* template<typename Tp, typename Compare> */
/* function<void(Tp&, Tp&)> Solution<Tp, Compare>::swap = [](Tp & lhs, Tp &rhs){ */
/*         Tp tmp = lhs; */
/*         lhs = rhs; */
/*         rhs = lhs; */
/*     }; */

void test01(){
    vector<int> vec = {1, 3, 8, 77, 4,32, 8, 7, 0};
    for_each(vec.begin(), vec.end(), [](const int &x){
                cout << x << " ";
             });
    cout << endl;
    Solution<int>().quickSort(vec, 0, vec.size() - 1);
    /* Solution<int, std::less<int>>().quickSort(vec, 0, vec.size() - 1); */
    /* vector<string> vec1 = {"hello", "world", "kim"}; */
    vector<string> vec1 = {"hello", "aorld", "zim", "b"};
    Solution<string, std::greater<string> >().quickSort(vec1, 0, vec1.size() - 1);
    for_each(vec1.begin(), vec1.end(), [](const string &x){
                cout << x << " ";
             });
    cout << endl;

    
}
int main(int argc, char* argv[])
{
    test01();

    return 0;
}

