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

#ifdef __cplusplus
extern "C"{
#endif

//命名空间
namespace wd{

int add(int, int);

void print(){
    cout << "print" << endl;
}
}//end of namespace wd  

int wd::add(int a, int b){
    return a + b;
}

#ifdef __cplusplus
}//end of extern "c"
#endif
using wd::print;


class Solution {
public:

};
int main()
{
    print();
    return 0;
}

