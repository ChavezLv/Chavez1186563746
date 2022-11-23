#include<iostream>
#include<unordered_set>
#include<vector>
using namespace std;
class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        //第一步：将wordList放入哈希表中，便于判断某个单词是否在wordList中
        unordered_set<string>wordSet(wordList.begin(),wordList.end());
        if(wordSet.size()==0||wordSet.find(endWord)==wordSet.end()) return 0;

        //分别用左边和右边扩散的哈希表代表单向BFS里的队列，他们在双向BFS中交替使用
        unordered_set<string>beginVisited;
        beginVisited.insert(beginWord);
        unordered_set<string>endVisited;
        endVisited.insert(endWord);

        //第二步：执行双向BFS，左右交替扩散的步数之和为所求
        int step=1;
        while(!beginVisited.empty()&&!endVisited.empty()){
            //优先选择小的哈希表进行扩散，考虑的情况更少
            if(beginVisited.size()>endVisited.size()){
                unordered_set<string>tmp=beginVisited;
                beginVisited=endVisited;
                endVisited=tmp;
            }
            //保证beginVisited是相对较小的集合，nextLevelVisited在扩散完成以后，会成为新的beginVisited
            unordered_set<string>nextLevelVisited;
            for(string word:beginVisited){
                for(int i=0;i<(int)word.size();++i){
                    string str=word;
                    for(char c='a';c<='z';++c){
                        if(str[i]==c) continue;
                        str[i]=c;
                        //在另外一边的BFS中汇合
                        if(endVisited.find(str)!=endVisited.end()){
                            return step+1;
                        }
                        //字符变化后在wordList中
                        if(wordSet.find(str)!=wordSet.end()){
                            nextLevelVisited.insert(str);
                            wordSet.erase(str);
                        }
                    }
                }
            }
            beginVisited=nextLevelVisited;
            ++step;
        }
        return 0;
    }
};
