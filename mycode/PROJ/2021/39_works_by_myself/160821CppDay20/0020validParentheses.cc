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

#include <stack>
using std::stack;

class Solution {
public:
    bool isValid(string s) {
        std::stack<char> inputStack;

        for(auto &ch : s){
            switch(ch){
            case '{':
                inputStack.push(ch);
                break;
            case '[':
                inputStack.push(ch);
                break;
            case '(':
                inputStack.push(ch);
                break;
            case ')':
                if(!inputStack.empty() && inputStack.top() == '('){
                    inputStack.pop();
                }else{
                    return false;
                }
                break;
            case '}':
                if(!inputStack.empty() && inputStack.top() == '{'){
                    inputStack.pop();
                }else{
                    return false;
                }
                break;
            case ']':
                if(!inputStack.empty() && inputStack.top() == '['){
                    inputStack.pop();
                }else{
                    return false;
                }
                break;
            }
        }
        return inputStack.empty();
    }
    
};
void test01(){
    cout << Solution().isValid("()[]{}") << endl;;
    cout << Solution().isValid("()") << endl;;
    cout << Solution().isValid("([]{}") << endl;;
    cout << Solution().isValid("(]") << endl;;
    cout << Solution().isValid("]") << endl;;
}
int main(int argc, char* argv[])
{

    test01();
    return 0;
}

