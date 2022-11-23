#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
/* using namespace std; */
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::stringstream;

class Solution {
public:

};
string word;
#define LOG(format, ...) printf(format, __VA_ARGS__)
#define DB(format, ...)  string("format") + string(__VA_ARGS__); 
#define DBf(format, ...)  {\
        stringstream os;\
        char str[100];\
        sprintf(str, format, __VA_ARGS__);\
        os >> str;\
cout << str << endl;}
int main()
{
    LOG("hello, %d", 123);
    cout << DB("eh", "asdf");
    DBf("asdf%d", 1234);
    cout << endl;
    return 0;
}
