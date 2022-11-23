#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <stack>
using namespace std;

/* template<typename _Tp> */
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution {
public:
    bool isSameTree(TreeNode* p, TreeNode* q){
        if(p->val == q->val){
            if(p->left == nullptr && p->right == nullptr && \
               q->left == nullptr && q->right == nullptr){
                /* return true; */
                _stack.push(true);
            }else if(p->left == nullptr && q->left == nullptr){
                bool judge = isSameTree(p->right, q->right);
                if(judge){
                    /* return true; */
                    _stack.push(true);
                }else{
                    _stack.push(false);
                    /* return false; */
                }
            }else if(p->right == nullptr && q->right == nullptr){
                bool judge = isSameTree(p->left, q->left);
                if(judge){
                    return true;
                }else{
                    return false;
                }
            }else{
                if(isSameTree(p->left, q->left) == false || \
                   isSameTree(p->right, q->right) == false){
                    return false;
                }else{
                    return true;
                }
            }
        }else{
            return false;
        }
        
    }
    bool isSameTree1(TreeNode* p, TreeNode* q){
        if(p->val == q->val){
            if(p->left == nullptr && p->right == nullptr && \
               q->left == nullptr && q->right == nullptr){
                return true;
            }else if(p->left == nullptr && q->left == nullptr){
                bool judge = isSameTree(p->right, q->right);
                if(judge){
                    return true;
                }else{
                    return false;
                }
            }else if(p->right == nullptr && q->right == nullptr){
                bool judge = isSameTree(p->left, q->left);
                if(judge){
                    return true;
                }else{
                    return false;
                }
            }else{
                if(isSameTree(p->left, q->left) == false || \
                   isSameTree(p->right, q->right) == false){
                    return false;
                }else{
                    return true;
                }
            }
        }else{
            return false;
        }
    }

    
private:
    stack<function<void> > _stack;
   
};
void test01(){
    TreeNode* p = nullptr;
    TreeNode* q = nullptr;
    TreeNode a(1);
    TreeNode b(0);
    TreeNode c(1, &a, &b);
    TreeNode d(1, &a, &b);
    TreeNode e(2, &c, &d);
    TreeNode f(2, &c, &d);
    cout << Solution().isSameTree(&a, &b) << endl;
    cout << Solution().isSameTree(&c, &d) << endl;
    cout << Solution().isSameTree(&e, &f) << endl;
}
int main(int argc, char* argv[])
{
    test01();
    return 0;
}
