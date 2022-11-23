#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

/* template<typename _Tp> */
class Solution {
public:
    static bool cmp(const int &a, const int &b){
        return abs(a) > abs(b);
    }
    int largestSumAfterKNegations(vector<int> & A, int K){
        sort(A.begin(), A.end(), cmp);
        /* int minPositiveNumber = INT32_MAX; */
        for(int i = 0; i < (int)A.size(); ++i){
            if(A[i] < 0 && K > 0){
                A[i] *= -1;
                --K;
            }
            /* if(A[i] >= 0){ */
            /*     minPositiveNumber = min(minPositiveNumber, A[i]); */
            /* } */
        }
        if(K % 2 == 1){
            A[A.size() - 1] *= -1;
        }
        int result = 0;
        for(int & a : A){
            result += a;
        }
        return result;
    } 
   
};
void test01(){
    vector<int> A = {2,-3,-1,5,-4};
    cout << Solution().largestSumAfterKNegations(A, 2) << endl;
}
int main(int argc, char* argv[])
{
    test01();
    return 0;
}
