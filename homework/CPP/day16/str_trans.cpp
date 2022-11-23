#include<iostream>
#include<map>
#include<fstream>
#include<string>
using namespace std;
class Trans{
public:
    Trans(ifstream &fs1,ifstream& fs2){
    //stream对象本身不可拷贝，能通过传引用的方式传给函数；
        buildMap(fs1);
        trans(fs2);
    }
    void trans(ifstream& transFile){
        string strbuff;
        while(transFile>>strbuff){
            string strbuff2;
            int size=strbuff.length();
            for(int i=0;i<size;++i){
            //cout<<strbuff<<endl;
                if((strbuff[i]>='a'&&strbuff[i]<='z')\
                   ||(strbuff[i]<='Z'&&strbuff[i]>='A')||strbuff[i]=='-')
                    {
                            strbuff2+=strbuff[i];
                    }
            }
            auto it=_mapInfo.find(strbuff2);
            if(it!=_mapInfo.end()){
                strbuff2.clear();
                strbuff2=it->second;
            }
            cout<<strbuff2<<" ";
        }
        cout<<endl;
    }
    void buildMap(ifstream &_mapFile){
        string key,value;
        while(_mapFile.good()&&_mapFile>>key\
              &&_mapFile.get()&&getline(_mapFile,value)){
            if(key.empty()&&value.empty())return;
            _mapInfo.insert({key,value});
            //auto it=_mapInfo.end();
            //cout<<(--it)->first<<":"<<(--it)->second<<endl;
        }
    }
    void print(){
        for(auto elem:_mapInfo){
            cout<<elem.first<<":"<<elem.second<<endl;
        }
    }
private:
    map<string,string>_mapInfo; 
    //ifstream _mapFile;
    //ifstream _transFile;
};
int main(){
    ifstream fsa,fsb;
    fsa.open("a.txt");
    fsb.open("b.txt");
    Trans ts(fsa,fsb);
    ts.print();
    return 0;
}
