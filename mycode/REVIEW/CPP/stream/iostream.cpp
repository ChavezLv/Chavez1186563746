#include<iostream>
#include<fstream>
using namespace std;
void test1(){
    auto stream=cin.tie();
    cout<<"stream:"<<stream<<endl;
    cout<<"&cout:"<<&cout<<endl;
    stream=cin.tie(nullptr);
    cout<<"stream:"<<stream<<endl;
    cout<<"&cout:"<<&cout<<endl;
}
void test2(){
    ifstream ifs("a.txt");
    if(!ifs.is_open()){
        cerr<<"ifstream open file error!\n";
        return ;
    }
    //position in the end;
    ifs.seekg(std::ios_base::end);
    //return the offset of the file pointer;
    auto len=ifs.tellg();//?获取长度出错
    cout<<len<<endl;
    ifs.seekg(std::ios_base::beg);
    char* buff=new char[len]();
    //read all content;
    ifs.read(buff,len);
    cout<<buff<<endl;
    //storage on a type of string which called content;
    string content(buff,len);
    cout<<content<<endl;
    delete []buff;
}
void test3(){
    fstream in("a.txt",std::ios::in|std::ios::binary);
    if(!in.is_open()){
        cerr<<"fstream open file error\n";
        return ;
    }
    fstream out("b.txt",std::ios::out|std::ios::binary);
    if(!out.is_open()){
        cerr<<"fstream open file error\n";
        return ;
    }
    out<<in.rdbuf();
    out.close();
    in.close();
}
int main(){
    test3();
    return 0;
}
