#include<iostream>
#include<map>
using namespace std;
void test(){
    std::multimap<string, string> pets; // Element is pair{pet_type, pet_name}
    auto iter = pets.insert (std::pair<string, string>{string{"dog"}, string{"Fang"}});
    iter = pets.insert(iter, std::make_pair("dog", "Spot")); // Insert Spot before Fang
    pets.insert(std::make_pair("dog", "Rover"));// Inserts Rover after Fang
    pets.insert (std::make_pair ("cat", "Korky"));// Inserts Korky before all dogs
    pets.insert ({{ "rat", "Roland"}, {"pig", "Pinky" }, {"pig", "Perky"}});//Inserts list elements
}
void test1(){
    multimap<string,pair<int,int>>mp;
    for(int j=0;j<7;++j){
        int t=2*j;
        mp.insert(pair<string,pair<int,int>>("wtf",make_pair(j,t)));
    }
    mp.insert(pair<string,pair<int,int>>("wang",make_pair(4,5)));
if(mp.end()!=mp.find("wang")){
                    ++mp.find("wang")->second.first;                                                                                   
                } 
               // strMap.insert(pair<string,pair<int,int>>\
                    (strBuffer2,make_pair(Repeat,it->_line)));
    for(auto itr=mp.begin();itr!=mp.end();++itr){
       cout<<itr->first<<":amount="<<itr->second.first\
               <<",line="<<itr->second.second<<"\t";
        cout<<"0"<<endl;
    }

}
int main(){
    test1();
    return 0;
}
