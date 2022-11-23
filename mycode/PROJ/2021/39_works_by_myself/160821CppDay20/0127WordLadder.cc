#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <deque>
using std::deque;

#include <list>
using std::list;

#include <map>
using std::map;

#include <set>
using std::set;

#include <algorithm>
//copy
#include <iterator>
using std::insert_iterator;
/* using std::ostream_iterator; */
// back_inserter()
#include <unordered_map>
using std::unordered_map;
#include <unordered_set>
using std::unordered_set;
#include <queue>
using std::queue;

class Solution {
private:
    unordered_set<string> _wordList;// find O(1), vector没有find
public:
    int ladderLength(string beginWord, string endWord, \
                      vector<string> & wordList) {
        /* _wordList = unordered_set<string>(wordList.begin(), wordList.end()); //方式二, 可能不是很好*/
        copy(wordList.begin(), wordList.end(), \
             insert_iterator<unordered_set<string>>(_wordList, _wordList.begin()  ) );//方式一
        /* copy(_wordList.begin(), _wordList.end(), std::ostream_iterator<string>(cout, " ")); */
        /* cout << endl; */

        unordered_set<string>::iterator iterWordList = _wordList.find(endWord);
        if(iterWordList == _wordList.end()){
            return 0;//如果没有endWord, 直接返回
        }

        /* unordered_map<string, int> mapWord;//word, path */

        //queue方式一
        /* queue<string> que; */
        //BFS最短路，每次都把当前的队列里能向前走的节点向前走一步，最先走到的就是最短的
        /* que.push(beginWord); */

        //queue方式一
        queue<std::pair<string, int>> que;
        que.push({beginWord, 1});
        _wordList.erase(beginWord);//已经进入队列的, 从字典中溢出

        int step = 0;//层数(步数)
        string tmpWord;
        while(!que.empty()){
            tmpWord = que.front().first;
            step = que.front().second;

            que.pop();
            for(size_t i = 0; i < tmpWord.size(); ++i){
                string bfsWord = tmpWord;
                for(char c = 'a'; c <= 'z'; ++c){
                    bfsWord[i] = c;
                    if(bfsWord == endWord){
                        //找到了, 返回当前走的步数+1
                        return step + 1;
                    }
                    if(_wordList.find(bfsWord) != _wordList.end()){
                        que.push({bfsWord, step + 1});
                        _wordList.erase(bfsWord);//删除该节点
                        
                    }

                }
            }
        }
        

        return 0;
    }

};
void test01(){
    string beginWord = "hit";
    string endWord = "cog";
    vector<string> wordList = {"hot","dot","dog","lot","log","cog"};
    /* vector<string> wordList = {"hot","dot","dog","lot","log"}; */
    /* set<string> wordList = {"hot","dot","dog","lot","log","cog"}; */
    /* copy(wordList.begin(), wordList.end(), std::ostream_iterator<string>(cout, " ")); */
    cout << Solution().ladderLength(beginWord, endWord, wordList) << endl;;
    
}
int main(int argc, char* argv[])
{

    test01();
    return 0;
}

