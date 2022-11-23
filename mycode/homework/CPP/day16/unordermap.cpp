#include<iostream>
#include<fstream>
#include<unordered_map>
using namespace std;
class unMap{
public:
    unMap(string fn):_filename(fn){
        loadFile();
    }
    void loadFile(){
        ifstream ifs;
        ifs.open(_filename);
        string strbuff;

        if(ifs.good()){
        while(ifs>>strbuff){
            string strbuff2,strbuff3;
            int size=strbuff.length();
            bool flag=true;
            for(int i=0;i<size;++i){
            //cout<<strbuff<<endl;
                if((strbuff[i]>='a'&&strbuff[i]<='z')\
                   ||(strbuff[i]<='Z'&&strbuff[i]>='A')||strbuff[i]=='-')
                    {
                        if(flag)
                            strbuff2+=strbuff[i];
                        else{
                            strbuff3+=strbuff[i];
                        }
                    }else{
                        //有些单词不是以空格分隔，而是以标点符号
                        if(i!=size-1) {
                            flag=false;
                            }    
                    }
            }
            pair<string,int>pr(strbuff2,_count);   
            auto res=_unmap.insert(pr);
            if(!res.second){
                ++((*(_unmap.find(strbuff2))).second);
            }
            if(!strbuff3.empty()){
                auto res=_unmap.insert({strbuff3,_count});   
                if(!res.second){
                   ++((*(_unmap.find(strbuff3))).second);
                }

            }
        strbuff.clear();
        }
    }
}
    void print(){
        for(auto pr:_unmap){
            cout<<pr.first<<":"<<pr.second<<endl;
        }
        cout<<endl;
    }
    void find(const string& word ){
        auto it=_unmap.find(word);
        if(it!=_unmap.end())
            cout<<word<<":"<<(*it).second<<endl;
    }
private:
    unordered_map<string,int>_unmap;
    int _count=1;
    string _filename;
};
int main(){
    unMap ump("china-daily.txt"); 
    ump.print();
    ump.find("China");
    return 0;
}

