#include<iostream>
#include<vector>
#include<unordered_set>
#include<queue>
using std::cout;
using std::string;
using std::endl;
/*单向广度优先遍历(BFS)*/
struct Solution{
    std::vector<std::vector<int>>group;
    std::vector<int>d;
    bool check(string a,string b){
        int ctl=0;
        for(int i=0;i<(int)a.length();++i){
            if(a[i]!=b[i]){
                ctl++;
               }
        }
        return ctl==1;
    }
    std::queue<int>que;
    int func(string beginWord,string endWord,std::vector<string>&wordList){
        int n=wordList.size();
        if(!n){return 0;}
        d.assign(n+1,-1);
        group.assign(n+1,std::vector<int>());
        int end=-1;
        for(int i=0;i<n;++i){
            for(int j=i+1;j<n;++j){
                if(check(wordList[i],wordList[j])){
                    group[i].push_back(j);
                    group[i].push_back(n);
                    if(wordList[i]==endWord){
                        end=i;
                    }
                }
            }
        }

        return 0;
    }
};
int main (){
    std::vector<string>vec{
        "hot","dot","dog","lot","log","cog"};
    Solution s;
    cout<<s.func("hit","cog",vec)<<endl;
}
