#include <iostream>
#include<string>
#include<fstream>
#include<vector>
#include<algorithm>
#include<sstream>
#include<string.h>
#include<algorithm>
//#include<func.h>
using std::cout;
using std::cin;
using std::endl;
using std::cerr;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::string;
using std::fstream;
using std::ostringstream;
using std::istringstream;
using std::stringstream;
using std::iterator;
struct Record
{
    string _word;
    int _frequency;
};

class Dictionary
{
public:
    void read(const string &filename);
    void store(const string &filename);
private:
    vector<Record> _dict;
};
void Dictionary::read(const string &filename)
{
    int idx=0;
    ifstream ifs(filename);
    //cout<<filename<<endl;
    if(!ifs.good())
    {
        cerr<<"ifstream is not good"<<endl;
        return;
    }
    string line;
    int cnt=0;
    while(getline(ifs,line))
    {
        for(int i=0;i<(int)line.length();++i)
        {
            if((line[i]>='a'&&line[i]<='z')||(line[i]>='A'&&line[i]<='Z'))
            {
                continue;
            }
            else
            {
                line[i]=' ';
            }
        }
        ++cnt;
        if(cnt%50 == 0){
            cout << cnt << endl;
        }
        stringstream ss(line);
        string word;
        while(ss>>word)
        {
            for(int i=0;i<=idx;++i)
            {

                if(i == idx){
                    cout << "idx: " << idx << endl;

                }
                if(i<idx&&word==_dict[i]._word)
                {
                    ++_dict[i]._frequency;
                }
                else if(i<idx&&word.compare(_dict[i]._word)<0)
                {
                    continue;
                }else
                {
                    struct Record temp={word,1};
                    auto it=_dict.begin();
                    _dict.insert(it+i,temp);
                    ++idx;
                    break;
                }
            }
        }
    }
}
void Dictionary::store(const std::string &filename)
{
    cout<<111<<endl;
    ofstream ofs(filename);
    if(!ofs.good())
    {
        cerr<<"ofstream is not good"<<endl;
        return;
    }
    for(auto it=_dict.begin();it!=_dict.end();it++)
    {
        ofs<<it->_word<<it->_frequency<<endl;
    }
    ofs.close();
}
int main(int argc,char *argv[])
{
    Dictionary dirc;
    dirc.read(argv[1]);
    dirc.store("txt.txt");
    return 0;
}

