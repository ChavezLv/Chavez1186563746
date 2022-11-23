#include<iostream>
#include<vector>
#include<unordered_set>
#include<queue>
using std::cout;
using std::string;
using std::endl;
/*单向广度优先遍历(BFS)*/
struct Solution{
    int func(string beginWord,string endWord,std::vector<string>&wordList){
        std::unordered_set<string>wordSet(wordList.begin(),wordList.end());
        if(wordSet.empty()||wordSet.find(endWord)==wordSet.end()){
            return 0;
        }
        std::queue<string>que;
        que.push(beginWord);
        //unordered_set<string>visited;
        int step=1;
        while(!que.empty()){
            int n=que.size();
            while(n--){
                string currWord=que.front();
                que.pop();
                for(int i=0;i<(int)currWord.size();++i){
                    char sourceChar=currWord[i];
                    for(int j=0;j<26;++j){
                        if(char('a'+j)==sourceChar){continue;}
                        currWord[i]=char('a'+j);
                        if(wordSet.find(currWord)!=wordSet.end()){
                            if(currWord==endWord)
                            {   
                                return step+1;//终止条件;
                            }else{
                                que.push(currWord);
                                wordSet.erase(currWord);
                            }
                        }
                    }
                }
            }
            ++step;
        }
       return 0; 
    }
};
