#include<iostream>
#include<vector>
using namespace std;
int main(){
    vector<int>vec{1,5,6,3,7,4};
    auto it=vec.end();
    cout<<it<<":"<<&*it<<":"<<*it<<endl;
    return 0;

}
