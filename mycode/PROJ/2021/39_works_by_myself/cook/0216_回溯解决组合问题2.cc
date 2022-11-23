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
// for_each
//
#include <numeric>
//accumulate, 体积很小，只包括几个在序列上面进行简单数学运算的模板函数
#include <functional>

class Solution {
public:
    //回溯的核心:
    /* (1)所有回溯法的问题都可以抽象为树形结构！
     * (2)回溯法解决的都是在集合中递归查子集， 集合的大小就构成了树的宽度, \
      递归的深度，都构成的树的深度。
     * (3)for循环可以理解是横向遍历， backtracking（递归）就是纵向遍历
     *
     * */

    //回溯法模板: 
    /*
     * (1)回溯函数模板返回值以及参数: 返回值 bk(参数){  }
     *      本题: void backTracking(int n, int k, int startIndex){}
     * (2)回溯函数终止条件: if(可以终止){存结果 + return}
     *      本题: if((int) path.size() == k && n == accumulate(path.begin(), path.end(), 0)){  }
     * (3)回溯搜索的遍历过程: for(树的某一层元素集合){ 处理节点; 递归; 回溯处理结果; }
     *      本题: for(int pathNode = startIndex; pathNode <= 9; ++pathNode){  }
     *
     * */

    //剪枝
    /*
     * 可以放到for 循环里面
     * 剪枝思路: 因为"for(树的某一层元素集合)", 思考, 这个集合是不是太大了?
     * 剪枝本质: 剪枝就是剪树节点的子孩子
     *
     * 方便思考, 更好理解, 可以做的对于剪枝的改进: 
     * void backtracking(int targetSum, int k, int sum, int startIndex){  }
     * 可以添加一个参数, 这样就不用计算了, 更直观, 每次push的同时sum += i;
     * pop的同时sum -= i;
     *
     * 所以说, 回溯法中的回溯函数一般先写好逻辑, 参数可以灵活调整.
     *
     */
    //k = 3, n = 7
    vector<vector<int>> combinationSum3(int k, int n) {
        result.clear();
        path.clear();
        //          (7, 3, 1)
        backTracking(n, k, 1);
        return result;
    }
    void backTracking(int n, int k, int startIndex){
        //剪枝
        //开始的下标 + path的sum 已经大于 n
        
        //if  start开始计算的 k - path.size() 个数字的和  > n - 当前的和, 则退出
        //比如: 7 和 3作为传入, 
        // 2 + 3 + 4 > 7
        // 1 + 2 时 size 为 2, sum为3, 下一步, start为 3-->9 , 当pathNode为5时, 2 * 5 + 3 - 2 -1 > 2 * (7 - 3),  就是 5 > 4了! 

        int sum = accumulate(path.begin(), path.end(), 0);
        //终止条件
        if((int) path.size() == k){
            if(n == sum){
                result.push_back(path);
            }
            return;
        }else{
            int sum = accumulate(path.begin(), path.end(), 0);
            
            if((2 * startIndex + k - (int)path.size() - 1) * (k - (int)path.size()) > 2 * (n - sum)){
                return;
            }
            
        }

        for(int pathNode = startIndex; pathNode <= 9; ++pathNode){
            path.push_back(pathNode);
            backTracking(n, k, pathNode + 1 );
            path.pop_back();
        }
    }
private:
    vector< vector<int> > result;//存放符合条件的各个组合结果的集合
    vector< int > path;//存放符合条件的单个组合结果

};
void test01(){
    vector< vector<int> > vec = Solution().combinationSum3(3, 9);
    /* vector< vector<int> > vec = Solution().combinationSum3(4, 1); */
    for_each(vec.begin(), vec.end(), [](auto &elem){
             for_each(elem.begin(), elem.end(), [](auto &x){
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

