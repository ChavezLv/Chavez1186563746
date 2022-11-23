#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

/* template<typename _Tp> */
class Solution {
public:
    bool qualifyEndian(){
        int test = 0x12345678;
        if((test & 0xff) == 78){
            return true;
        }else{
            return false;
        }

    }
    bool judgeEndian(){
        // 10 00 0000 00000000
        /* int move = 0x8; */
        int test = 0x12345678;
        cout << hex << (test & 0xff) << endl;
        if((test & 0xff ) == 78){
            return true;
        }else{
            return false;
        }
        /* for(int i = 0; i < 4; ++i){ */
        /*     move = move << 4; */
        /* } */
        /* if(low != 0){ */
        /*     return true; */ 
        /* }else{ */
        /*     return false; */
        /* } */
    }

    unsigned long toByteUnits(const string &input){
        if(0 == input.size()){
            return 0;
        }
        if(isLegal(input)){
            if(input[input.size() - 1] == 'K'){
                int temp = strToNum(input, input.size() - 1);
                /* cout << "K: " << temp << endl; */
                return temp * 1024;
            }
            else if(input[input.size() - 1] == 'M'){
                int temp = strToNum(input, input.size() - 1);
                if(temp == -1){
                    return -1;
                }
                return temp * 1024 * 1024;
            }
            else if(input[input.size() - 1] == 'G'){
                int temp = strToNum(input, input.size() - 1 );
                if(temp == -1){
                    return -1;
                }
                long long tmp = temp;
                /* int a = 1 << 10; */
                /* cout << a << endl; */
                
                /* cout <<  (1024 * 1024 * 1024 * 2)<< endl; */
                /* unsigned int multi1 = 1024 * 1024 * 1024; */
                cout << "INT32_MAX: " << INT32_MAX / 2<< endl;
                /* cout << "1024 * 1024 * 1024 : " << multi << endl; */
                /* cout << multi * tmp << endl; */
                /* multi += multi; */
                /* multi += multi / 2; */
                unsigned int maxInt = 0xffffffff;//4294967295
                cout << maxInt << endl;
                unsigned int multi = INT32_MAX / 2 + 1;
                unsigned long output = 0;//= multi * tmp;
                
                /* for(int start = 1; start <= temp; ++start){ */
                /*     output += multi; */
                /*     cout << "output : " << output << endl; */
                /* } */
                return (unsigned long)multi * tmp;
                /* return output; */
            }
            else{
                return strToNum(input, input.size());
            }
        }
        return -1;
        /* int num = strToNum(input); */
        /* return num; */
    }

    int strToNum(const string &input, int end){
        int temp = 0;
        int bit = 1;

        /* cout << "end: " << end << endl; */
        /* cout << input << endl; */
        for(int index = end - 1; index >= 0; --index){
            if(judgeBeyondMaxInt(temp, (input[index] - '0') * bit)){
                return -1;
            }
            temp += (input[index] - '0') * bit;
            bit *= 10;
        }
        
        return temp; 
    }
    bool judgeBeyondMaxInt(const int &lhs, const int &rhs){
        if(INT32_MAX - lhs < rhs){
            return true;
        }else{
            return false;
        }
    }

    bool isLegal(const string &input){
        int index = 0;
        bool flag = false;
        for(; index < (int)input.size() - 1; ++index){
            if(input[index] < '0' && input[index] > '9'){
                return false;
            }else if(input[index] == '.'){
                if(flag == false){
                    flag = true;
                }else{
                    return false;
                }
            }
        }
        if(input[index] == 'K' || input[index] == 'M' || input[index] == 'G'\
           || (input[index] >= '0' && input[index] <= '9')){
            return true;
        }else{
            return false;
        }
    }
};

void test01(){
    /* cout << Solution().judgeEndian(); */
    /* cout << Solution().toByteUnits("10K") << endl; */
    cout << Solution().toByteUnits("10G") << endl;
    /* cout << Solution().toByteUnits("100") << endl; */
}
int main(int argc, char* argv[])
{
    test01();
    return 0;
}
