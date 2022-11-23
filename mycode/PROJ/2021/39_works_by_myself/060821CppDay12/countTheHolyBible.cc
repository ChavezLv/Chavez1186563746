#include <string.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <iomanip>
/* #include <vector> */

/* using namespace std; */
using std::cin;
using std::cout;
using std::endl;
using std::string;
/* using std::vector; */
using std::ios;
using std::pair;
using std::map;

/* using std::fstream; */
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::setiosflags; 
using std::setw;

/* class Record{ */
/* struct Record{ */
/*     string _word; */
/*     int _frequency; */
/* }; */

/* bool cmp(string str1, string str2){ */
/* bool cmp(Record rec1, Record rec2){ */
/*     return rec1._word < rec2._word ? true : false; */
/* } */

class Dictionary {
public:
    //Dictionary();
    //...
    void read(const string &filename);
    bool judgeWord(string word);
    bool findWordAndAdd(string word);
    void store(const string &filename);
    /* void test(){ */
    /*     /1* Record rec; *1/ */
    /*     /1* for(auto &tmp: _dict){ *1/ */
    /*         for(auto &elem: _dict){ */
    /*             cout << elem.first << " " << elem.second << endl; */
    /*         } */
            
    /*     /1* } *1/ */
    /* } */
private:
    /* vector<Record> _dict; */
    map<string, int> _dict;
};

/* Dictionary::Dictionary(){ */
/*     /1* _dict.reserve(128); *1/ */
/* } */

void Dictionary::read(const string &filename){
    ifstream ifs(filename, std::ios::in);//只读, 不存在打开失败
    if(!ifs.good()){
        operator<<(cout, "file not exist");
        return;
    }

    string line;
    string words;
    stringstream ss(line);
    while(getline(ifs, line)){
        ss.clear();
        ss << line;
        while(ss >> words){
            //比下面更快
            if(judgeWord(words)){
                auto ret = _dict.insert(pair<string, int>(words, 1));
                if(!ret.second){
                    //如果插入不成功
                    ++ _dict[words];
                }
            }
            /* if(judgeWord(words)){//如果是单词,则放入map中 */
            /*     auto it = _dict.find(words); */
            /*     if(it == _dict.end()){ */
            /*         //如果没有该单词, 插入 */
            /*         _dict.insert(pair<string, int>(words, 1)); */
            /*     }else{ */
            /*         //否则,词频+1 */
            /*         ++ it->second; */
            /*     } */

            /* } */
        }
    }
    /* sort(_dict.begin(), _dict.end(), cmp); */
    ifs.close();
}

bool Dictionary::judgeWord(string word){
    for(auto &ch: word){
        /* if(!(ch >= 'a'&& ch <='z') && !(ch >= 'A' && ch <= 'Z')){ */
        if(!isalpha(ch)){
            return false;
        }
    }
    return true;
}
void Dictionary::store(const string &filename){
    ofstream ofs(filename);
    if(!ofs){
        cout << "error" << endl;
        return ;
    }
    stringstream ss;
    for(auto &rec: _dict){
        ofs << setiosflags(ios::left) << setw(20) <<  rec.first;
        ofs.width(20);
        ofs << rec.second << endl;
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
    time_t begin = time(nullptr);
    dic.read(str1);
    dic.store(str2);
    time_t end = time(nullptr);
    cout << end - begin << endl;
    return 0;
}
