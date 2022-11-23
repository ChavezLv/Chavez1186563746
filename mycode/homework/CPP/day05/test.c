#include<iostream>
using namespace std;
int main(){
    //char *str;//野指针,coredump错误
    //cin>>str;
    //cout<<str<<endl;
    
    char a[200] = {0};
    cin.getline(a, 5, ' ');
    cout << a;
}
