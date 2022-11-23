#include <stdlib.h>
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
     *      本题: vector<string> letterCombinations(string digits){}
     * (2)回溯函数终止条件: if(可以终止){存结果 + return}
     *      本题: if(0 == digits.size()){  }
     * (3)回溯搜索的遍历过程: for(树的某一层元素集合){ 处理节点; 递归; 回溯处理结果; }
     *      本题: for(int pathNode = 0; pathNode < (int)vecChar.size(); ++pathNode){  }
     *
     * */

    //剪枝(0017不用剪枝)
    /*
     * 可以放到for 循环里面
     * 剪枝思路: 因为"for(树的某一层元素集合)", 思考, 这个集合是不是太大了?
     * 剪枝本质: 剪枝就是剪树节点的子孩子
     *
     * 有些题目为了更直观显示思考过程, 更好理解, 可以做的对于剪枝的改进: 
     * 比如0216题中: void backtracking(int targetSum, int k, int sum, int startIndex){  }
     * 可以添加一个参数, 这样就不用计算了, 更直观, 每次push的同时sum += i,pop的同时sum -= i;
     * 所以说, 回溯法中的回溯函数一般先写好逻辑, 参数可以灵活调整.
     *
     */

    Solution()
        : num2alpha(
        { {}, {}, {'a', 'b','c'}, {'d', 'e', 'f'}, \
        {'g', 'h', 'i'}, {'j', 'k', 'l'}, {'m', 'n', 'o'},\
        {'p', 'q', 'r', 's'}, {'t', 'u', 'v'}, {'w', 'x', 'y', 'z'}}
                   ){
        
    }
    vector<string> letterCombinations(string digits) {
        result.clear();
        path.clear();
        if(0 == digits.size()){
            return result;
        }
        backTracking( digits, 0);
        return result;
    }
    void backTracking(string &digits,int digitsStartIndex){
        //剪枝

        //终止条件
        if(path.size() == digits.size()){
            
            result.push_back(path);
            return;
        }

        // int index = digits[digitsStartIndex] - '0';
        // vector<char> vecChar = num2alpha[index];
        vector<char> vecChar = num2alpha[digits[digitsStartIndex] - '0'];
        //for
        for(int pathNode = 0; pathNode < (int)vecChar.size(); ++pathNode){
            path.push_back(vecChar[pathNode]);
            backTracking(digits, digitsStartIndex + 1);
            path.pop_back();
        }
    }
private:
    vector< vector<char> > num2alpha;
    vector< string > result;//存放符合条件的各个组合结果的集合
    string path;//存放符合条件的单个组合结果
};

void test01(){
    /* vector< string >  vec = Solution().letterCombinations("22"); */
    vector< string >  vec = Solution().letterCombinations("");
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

