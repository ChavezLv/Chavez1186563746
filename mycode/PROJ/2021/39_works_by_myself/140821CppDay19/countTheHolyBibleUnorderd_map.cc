#include <math.h>
#include <sys/time.h>
#include <string.h>
#include <iomanip>
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

#include <unordered_map>
using std::unordered_map;

#include <fstream>
using std::ifstream;
using std::fstream;

#include <sstream>
using std::stringstream;


class  Dictionary{
public:
    void read(const string &fileInput);
    bool judgeWord(string word);
    void store(const string &fileOuput);

private:
    unordered_map<string, int> _dict;
};

void Dictionary::read(const string &fileInput){
    ifstream ifs(fileInput, std::ios::in);
    if(!ifs){
        operator<<(cout, "file doesn't exist");
        return;
    }

    string line;
    string word;
    stringstream ss;
    while(getline(ifs, line)){
         ss.clear();
         ss << line;
         while(ss >> word){
             if(judgeWord(word)){
                 auto ret = _dict.insert({word, 1});
                 if(!ret.second){
                     ++ _dict[word];
                 }
             }
         }
    }
    ifs.close();
}

bool Dictionary::judgeWord(string word){
    for(auto &ch : word){
        if(!isalpha(ch)){
            return false;
        }
    }
    return true;
}

void Dictionary::store(const string &fileOuput){
    fstream fs(fileOuput, std::ios::out);
    if(!fs){
        cout << "open error" << endl;
        return ;
    }
    stringstream ss;
    for(auto &item : _dict){
        fs << setiosflags(std::ios::left) << std::setw(20) << item.first \
            << std::setw(20) << item.second << endl;
    }
    fs.close();
}
    
void test01(char *argv[]){
    Dictionary dic;
    struct timeval start, end;
    memset(&start, 0, sizeof(start));
    memset(&end, 0, sizeof(start));
    gettimeofday(&start, nullptr);

    dic.read(argv[1]);
    dic.store(argv[2]);
    
    gettimeofday(&end, nullptr);
    cout << "time: " << (end.tv_sec - start.tv_sec) * pow(10, 6) \
        + end.tv_usec - start.tv_usec << " us"<< endl;
    
}
int main(int argc, char* argv[])
{
    if(argc != 3){
        cout << "args error " << endl;
        return 0;
    }
    test01(argv);
    return 0;
}

