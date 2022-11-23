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
int main()
{
    string str1 = "helloworld";
    str1;
    /* char *p = const_cast<char *>(str1.c_str()); */
    char *p = (char*)str1.c_str();
    return 0;
}

