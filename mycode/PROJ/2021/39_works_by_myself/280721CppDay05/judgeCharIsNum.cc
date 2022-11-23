#include <iostream>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cin;
using std::cout;
using std::endl;

class Solution {
public:
    bool judgeWrongxxxxxxxxx(char *pstr){
        /* char* cNum = (char*) calloc(1, 30); */
        /* int cNumPos = 0; */
        //          .   e   
        int flag[2] = {0, 0};
        for(int i = 0; pstr[i] != '\0'; ++i){
            if((pstr[i] == '+' || pstr[i] == '-')){
                if(pstr[i + 1] < '0' || pstr[i + 1] > '9'){
                    //+/- 后不为数字判为非
                    return false;
                }else{
                    //+/- 后是数字, 但非首位且前面不为e/E
                    if(i != 0 && (pstr[i - 1] != 'e' && pstr[i - 1] != 'E')){
                        return false;
                    }
                } 
            }else if(pstr[i] >= '0' && pstr[i] <= '9'){
                if(pstr[i + 1] != '\0' && (pstr[i + 1] < '0' && pstr[i + 1] > '9') && (pstr[i + 1] \
                   != '.'&&pstr[i + 1] != 'e'&& pstr[i + 1] != 'E')){
                    return false;
                /* }else if(pstr[i + 1] != '\0'){ */
                /*     break; */
                }
            }else if(pstr[i] == '.'){
                if(flag[0] == 1 || pstr[i + 1] < '0' || pstr[i + 1] > '9'){
                    return false;
                }
                flag[0] = 1;
            }else if(pstr[i] == 'e' && pstr[i] == 'E'){
                if(0 == i || flag[1] == 1){
                    return false;
                }else{
                    if(pstr[i + 1] < '0' || pstr[i + 1] > '9'){
                        return false;
                    }
                }
                flag[1] = 1;
            }else{
                return false;
            }
        }  

        return true;
    }
    bool judge(char* pstr) {
        /* char* cNum = (char*) calloc(1, 30); */
        /* int cNumPos = 0; */
        //.   e   
            int flag[2] = { 0, 0  };
        for (int i = 0; pstr[i] != '\0'; ++i) {
            if ((pstr[i] == '+' || pstr[i] == '-')) {
                if (pstr[i + 1] < '0' || pstr[i + 1] > '9') {
                    //+/- 后不为数字判为非
                    return false;
                }
                else {
                    //+/- 后是数字, 但非首位且前面不为e/E
                    if (i != 0 && (pstr[i - 1] != 'e' && pstr[i - 1] != 'E')) {
                        return false;
                    }
                }
            }
            else if (pstr[i] >= '0' && pstr[i] <= '9') {
                if (pstr[i + 1] != '\0' && (pstr[i + 1] < '0' || pstr[i + 1] > '9') && (pstr[i + 1] \
                                                                                        != '.' && pstr[i + 1] != 'e' && pstr[i + 1] != 'E')) {
                    return false;
                    /* }else if(pstr[i + 1] != '\0'){ */
                /*     break; */
            }
            }
            else if (pstr[i] == '.') {
                if (flag[0] == 1 || pstr[i + 1] < '0' || pstr[i + 1] > '9') {
                    return false;
                }
                flag[0] = 1;
            }
            else if (pstr[i] == 'e' || pstr[i] == 'E') {
                if (0 == i || flag[1] == 1) {
                    return false;
                }
                else {
                    if ((pstr[i + 1] < '0' || pstr[i + 1] > '9')&&pstr[i + 1] != '-'&&\
                        pstr[i + 1] != '+') {
                        return false;
                    }
                }
                flag[1] = 1;
            }
            else {
                return false;
            }
        }

        return true;
    }
};
int main()
{
    Solution sol;
    char *pstr = new char[64]();
    cin >> pstr;
    /* cout << sol.judge((char*)"1a3.14") << endl; */
    cout << sol.judge(pstr) << endl;
    return 0;
}

