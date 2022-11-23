#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

class Solution {
public:

};

void printVec(const std::vector<int>&vec){
    for(auto &i : vec){
        cout << i << " ";
    }
    cout << endl;
}

int main()
{
    /* vector<vector<int>> v(16, vector<int>(10));//16个 arr[10] */
    vector<vector<int>> v(16, vector<int>(10, 10));//16个 arr[10] 每个值都为10
    v[0].push_back(10);
    for(auto &i : v){
        for(auto &j : i){
            cout << j << " ";
        }
        cout << endl;
    }
    return 0;
}

