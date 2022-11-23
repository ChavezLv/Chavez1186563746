#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <map>
using std::map;

#include <unordered_map>
using std::unordered_map;

#include <fstream>
using std::ifstream;
using std::fstream;

#include <sstream>
using std::stringstream;
using std::istringstream;

class  WordTransform{
public:
    void readMap(const string &mapFile);
    const string & trans(const string &word);
    void print(const string &fileOuput);

private:
    unordered_map<string, string> _transMap;
    
};

void WordTransform::readMap(const string &mapFile){
    ifstream ifs(mapFile, std::ios::in);//失败说明不存在
    if(!ifs){//隐转
        throw std::runtime_error("transformation file doesn't exit");
        return;
    }
    string line;
    string word;
    string value;
    stringstream ss;
    while(getline(ifs, line)){
         ss.clear();
         ss << line;
         if(ss >> word){//只读第一个单词, 作为key
             while(ss >> line){//剩下读到value里面
                value = value + line + " ";
             }
             _transMap[word] = line;
         }
    }
    /* for(auto &elem : _transMap){ */
    /*     cout << elem.first << " " << elem.second << endl; */
    /* } */
    cout << endl;
    ifs.close();
}

const string & WordTransform::trans(const string &word){
    auto it = _transMap.find(word);
    if(it != _transMap.end()){
        //说明找到
        return _transMap[word];
    }
    return word;
}

void WordTransform::print(const string &fileOuput){
    ifstream ifs(fileOuput, std::ios::in);
    if(!ifs){
        throw std::runtime_error("file transformed doesn't exit");
        return;
    }
    string line;
    string word;
    while(getline(ifs, line)){
        istringstream ss(line); 
        while(ss >> word){
            cout << trans(word);
            cout << " ";
        }
        cout << endl;
    }
    ifs.close();

}
    
void test02(char *argv[]){
    WordTransform dic;
    dic.readMap(argv[1]);
    dic.print(argv[2]);
}

int main(int argc, char* argv[])
{
    if(argc != 3){
        cout << "args error " << endl;
        return 0;
    }
    test02(argv);
    return 0;
}

