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
void display(){
    cout << endl;
}
template<typename T, typename ... Args>
void display(T t, Args ... args){
    cout << t << " ";
    display(args...);
}
void test1(){
    display(1);
    display(2, 2.2);
    display('a', true, string("hello"));

}
int main()
{
    return 0;
}

