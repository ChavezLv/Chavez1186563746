#include <iostream>
#include <string>
#include <queue>
#include <vector>

using std::priority_queue;
using std::vector;
using std::string;
using std::cout;
using std::endl;

int main()
{
    string s1 = "a";
    string s2 = "b";
    priority_queue<string,vector<string>> pQue;
    pQue.push(s1);
    pQue.push(s2);

    while(!pQue.empty())
    {
        cout << pQue.top() << endl;
        pQue.pop();
    }
    return 0;
}

