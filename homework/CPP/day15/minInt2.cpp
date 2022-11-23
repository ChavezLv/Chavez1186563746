#include<iostream>
#include<unordered_set>
using namespace std;
class Solution {
public:
    Solution(int *arr,int size):_us(arr,arr+size){}
    int firstMissingPositive() {
        int result = 1;
        while (true){
            if(_us.end()!=_us.find(result)){
                result++;
            }else {
                break;
            }
        }
        return result;
    }
private:
        unordered_set<int> _us ;
    
};
int main(){
    int arr[5]={1,5,-2,2,3};
    Solution it(arr,5);
    cout<<"result:"<<it.firstMissingPositive()<<endl;
    return 0;
}
