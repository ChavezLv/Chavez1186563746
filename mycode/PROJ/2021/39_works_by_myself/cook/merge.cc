#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

class Solution {
public:
    bool merge(int a[], int a_len, int b[], int b_len, int c[], int c_len) {
        if(a_len + b_len > c_len){
            return false;
        }
        int i = 0;
        int j = 0;
        int k = 0;
        for(; i < c_len && j < a_len && k < b_len; ++i){
            if(a[j] < b[k]){
                c[i] = a[j];
                ++j;
            }else{
                c[i] = b[k];
                ++k;
            }
        }
        while(k < b_len && j == a_len){
            c[i] = b[k];
            ++k;
        }
        while(j < a_len && k == b_len){
            c[i] = a[k];
            ++a;
        }

        return true;
    }

   
};
void test01(){
       
}
int main(int argc, char* argv[])
{
       test01();
          return 0;
}
