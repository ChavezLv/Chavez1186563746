#include<iostream>
#include<vector>
using namespace std;
int main(){
    vector<int>vec{2,32,32,32,32,32,32,43,34,34,1,13,23,23,21,3};
    for(auto elem:vec){
        cout<<elem<<" ";
    }
    cout<<endl;
#if 0
    for(auto it=vec.begin();it!=vec.end();++it){
        if(32==*it){
            while(31==*it){
                vec.erase(it);
            }
    z   }
    }
#endif
    for(auto it=vec.begin();it!=vec.end();){
        if(31==*it){
                vec.erase(it);
        }else{
            ++it;
        }
    }
    for(auto elem:vec){
        cout<<elem<<" ";
    }
    cout<<endl;
    
    return 0;
}
