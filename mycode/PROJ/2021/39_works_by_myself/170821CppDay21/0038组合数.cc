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

class Solution {
public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        result.clear();
        path.clear();
        sort(candidates.begin(), candidates.end()); // 排序方便剪枝
        backtracking(candidates, target, 0, 0);
        return result;
    }

    void backtracking(vector<int>& candidates, int target, int sum, int startIndex) {
        if (sum == target) {
            result.push_back(path);
            return;

        }

        // 如果 sum + candidates[i] > target 就终止遍历
        for (int i = startIndex; i < (int)candidates.size() && sum + candidates[i] <= target; i++) {
            sum += candidates[i];
            path.push_back(candidates[i]);
            backtracking(candidates, target, sum, i);
            sum -= candidates[i];
            path.pop_back();

        }
    }
private:
    vector<vector<int>> result;
    vector<int> path;
};

void test01(){
   vector<int> vec = {2, 3, 6, 7};
   vector< vector<int> > vec1 = Solution().combinationSum(vec, 7);
   for_each(vec1.begin(), vec1.end(), [](auto &elem){
            for_each(elem.begin(), elem.end(), [](int &x){
                     cout << x << " ";
                     });
            cout << endl;
            });
}
int main(int argc, char* argv[])
{

    test01();
    return 0;
}

