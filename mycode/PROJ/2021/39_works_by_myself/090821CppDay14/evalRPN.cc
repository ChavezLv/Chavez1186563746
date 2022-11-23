#include <ctype.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <stack>
/* using namespace std; */
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::stack;

class Solution {
public:
    int evalRPN(vector<string>& tokens) {
        stack<int> numStack;
        int lhsNum = 0;
        int rhsNum = 0;
        for(auto &token : tokens){
            if(token.size() == 1 && !isdigit(token[0])){
                //如果只有一位且不是数字, 则为符号
                rhsNum = numStack.top();        
                numStack.pop();
                lhsNum = numStack.top();
                numStack.pop();
                switch(token[0]){
                    case '+':
                        numStack.push(lhsNum + rhsNum);
                        break;
                    case '-':
                        numStack.push(lhsNum - rhsNum);
                        break;
                    case '*':
                        numStack.push(lhsNum * rhsNum);
                        break;
                    case '/':
                        numStack.push(lhsNum / rhsNum);
                        break;
                }
            }else{
                //为数字, 入栈
                numStack.push(atoi(token.c_str()));        
            }
        }
        return numStack.top();
    }
};
int main()
{

    return 0;
}

