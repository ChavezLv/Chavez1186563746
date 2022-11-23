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
     *      本题: path.size() == k
     * (3)回溯搜索的遍历过程: for(树的某一层元素集合){ 处理节点; 递归; 回溯处理结果; }
     *      本题: for(int i = startIndex; i <= n; ++i){ //路径+1 }
     *
     * */
    vector<vector<int>> combine(int n, int k) {
        result.clear();
        path.clear();
        backTracking(n, k, 1);
        return result;
    }
    void backTracking(int n, int k, int startIndex){
        //传入4 3 
        //比较:
        //startIndex包括进去, 还有多少元素 vs path的大小size()还需要多少元素到k
        //4  - 3 + 1 =  2 vs  3 - 0(path.size()) = 能放 ;   3 2
        //剪枝
        /*
         * 可以放到for 循环里面
         * 剪枝思路: 因为"for(树的某一层元素集合)", 思考, 这个集合是不是太大了?
         * 剪枝本质: 剪枝就是剪树节点的子孩子
         *
         */
        //方式一:
        /* if(n - startIndex + 1 < k - (int)path.size()){ */
        /*     return; */
        /* } */
        if((int)path.size() == k){
            result.push_back(path);        
            return;
        }
        //
        for(int i = startIndex; i <= n + 1 - k + (int)path.size(); ++i){
            path.push_back(i);
            backTracking(n, k, i + 1);
            path.pop_back();
        }
    }
private:
    vector< vector<int> > result;//存放符合条件的各个组合结果的集合
    vector< int > path;//存放符合条件的单个组合结果

};
void test01(){
    vector< vector<int> > vec = Solution().combine(4, 2);
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

