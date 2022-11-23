#include <string.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::stringstream;
#define N 640
#define MAXKEY 1000

/* class Record{ */
struct Record{
    string _word;
    int _frequency;
};

int hash(const char *key)
{
    int h = 0, g;
    while (*key)
    {
        h = (h << 4) + *key++;
        g = h & 0xf0000000;
        if (g)
            h ^= g >> 24;
        h &= ~g;

    }
    return h % MAXKEY;

}
/* bool cmp(string str1, string str2){ */
bool cmp(Record rec1, Record rec2){
    return rec1._word < rec2._word ? true : false;
}
class Dictionary {
public:
    Dictionary();
    //...
    void read(const string &filename);
    bool judgeWord(string word);
    bool findWordAndAdd(string word);
    void store(const string &filename);
    void test(){
        /* Record rec; */
        /* for(auto &tmp: _dict){ */
            for(auto &rec: _dict){
                cout << rec._word << " " << rec._frequency << endl;
            }
            
        /* } */
    }
private:
    vector<Record> _dict;
    /* vector<vector<Record>> _dict; */
};

Dictionary::Dictionary(){
    _dict.reserve(128);
}

void Dictionary::read(const string &filename){
    ifstream ifs(filename, std::ios::in);//只读, 不存在打开失败
    if(!ifs.good()){
        operator<<(cout, "file not exist");
        return;
    }
    Record tmpRecord;
    string line;
    string words;
    stringstream ss(line);//每次都要重新初始化
    while(getline(ifs, line)){
        /* cout << line << endl; */
            /* cout << ss.bad() << " " << ss.eof() << endl; */
            /* cout << ss.tellg() << " " << ss.tellp() << endl; */
        ss.clear();
            /* cout << ss.bad() << " " << ss.eof() << endl; */
            /* cout << ss.tellg() << " " << ss.tellp() << endl; */
        ss << line;//每次都要重新初始化
        while(ss >> words){
            
            /* cout << words << endl; */
            /* memset(&tmpRecord, 0, sizeof(tmpRecord)); //core dump!*/
            tmpRecord._frequency = 0;
            tmpRecord._word = words;
            if(judgeWord(words)){
                ++tmpRecord._frequency;
                /* tmpRecord._frequency = 1; */
                if(!findWordAndAdd(words)){
                    _dict.push_back(tmpRecord); 
                }
            }
        }
    }
    sort(_dict.begin(), _dict.end(), cmp);
    ifs.close();
}
bool Dictionary::judgeWord(string word){
    for(auto &ch: word){
        if(!(ch >= 'a'&& ch <='z') && !(ch >= 'A' && ch <= 'Z')){
            return false;
        }
    }
    return true;
}
bool Dictionary::findWordAndAdd(string word){
    for(auto &rec: _dict){
        if(rec._word == word){
            ++rec._frequency;
            return true;
        }
    }
    return false;
}

void Dictionary::store(const string &filename){
    ofstream ofs(filename);
    if(!ofs){
        cout << "error" << endl;
        return ;
    }
    stringstream ss;
    for(auto &rec: _dict){
        ofs << rec._word << " " << rec._frequency << endl;
    }
    
    ofs.close();
}
int main()
{
    Dictionary dic;
    string str1;
    string str2;
    /* dic.read("test.txt"); */
    /* dic.store("store.txt"); */
    cin >> str1;
    cin >> str2;
    dic.read(str1);
    dic.store(str2);
    return 0;
}




