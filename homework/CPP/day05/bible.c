#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<unordered_map>
using namespace std;
class Dictionary{
public:
    void read(const std::string &filename);
    void store(const std::string &filename);
private:
    unordered_map<string,int>_dict;
};

void Dictionary::read(const std::string &filename){
    ifstream ifs(filename,ifstream::in);
    if(!ifs){
        cout<<"ifs open Error!"<<endl;
    }
    string word;
    cout<<"start to read"<<endl;
    while(ifs.good()){
        ifs>>word;
        _dict[word]++;//字典存储
    }
    cout<<"read over"<<endl;
    ifs.close();
}
void Dictionary::store(const string& filename){
    ofstream ofs(filename,ofstream::out);
    vector<pair<string,int>> vec(_dict.begin(),_dict.end());
    cout<<"start to write"<<endl;
    for(vector<pair<string,int>>::iterator it =vec.begin();it!=vec.end();it++)
    {
        string key=it->first;
        int value=it->second;
        ofs<<key<<" "<<value<<endl;
    }  
    cout<<"write end"<<endl;
    ofs.close();
}
int main(){
    string in="the_holy_bible.txt";
    string out="out.txt";
    Dictionary dict;
    dict.read(in);
    dict.store(out);
    return 0;
}
