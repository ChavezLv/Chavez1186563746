#include<iostream>
#include<unordered_map>
using namespace std;
int main(){
    unordered_map<string,int>ump{{"hello",555}};
    ump.insert({"world",444});
    cout<<(*(ump.find("hello"))).second<<endl;
    return 0;
}
