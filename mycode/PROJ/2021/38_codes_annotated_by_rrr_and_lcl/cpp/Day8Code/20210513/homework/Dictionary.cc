#include <time.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

using std::cout;
using std::endl;
using std::string;
using std::vector;//引入向量容器
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::sort;

struct Record //默认成员为public的record类
{
    Record(const string &word, int fre)//记录单词与词频
    : _word(word)//初始化内置类型直接写
    , _frequency(fre)
    {
        //可以的话写在初始化位置优于在内部写等号（上面的做法调用的是拷贝构造函数，下面的拷贝赋值函数）

    }
    string _word;//公开成员
    int _frequency;//公开成员
};

bool operator<(const Record &lhs, const Record &rhs)
{
    return lhs._word < rhs._word;//重载比较函数，左右操作数变为record类//应该是为了便于调用sort接口
}
class Dictionary //词典类/dict类
{
public:
    Dictionary(int capa)//初始化：传入参数【容量】
    {
        _dict.reserve(capa);//vector初始化特质：提前设定容量
    }
    void read(const std::string &filename);//读
    void store(const std::string &filename);//存储
    string dealWord(const string &word);//处理单词
    void insert(const string &word);//插入
private:
    vector<Record> _dict;//向量
};

void Dictionary::read(const std::string &filename)
{
    ifstream ifs(filename);//定义【读入】的input file stream
    if(!ifs)//等同于if(ifs.fail())
    {//打开检查
        std::cerr << "open " << filename << " error"<< endl;
        return;
    }

    string line;//字符串line//行为单位读入
    while(getline(ifs, line))//cpp函数，从文件流读行
    {
        istringstream iss(line);//定义input string stream，从字符串读
        string word;//定义word
        while(iss >> word)//字符流输出到string(分隔符是空白)
        {
            string newWord = dealWord(word);//处理非法单词
            //↑拿到非空白单词+处理非法单词（含非字母直接扔，或者字母替换空格）
            insert(newWord);///处理完new_word，使用成员函数insert
            //在一个类里，会现在类域找到函数
        }
    }

    sort(_dict.begin(), _dict.end());//利用_dict这个vetor的性质进行排序

    ifs.close();//关闭文件流
}//读取+预处理完毕

void Dictionary::store(const std::string &filename)
{
    ofstream ofs(filename);//负责【写入】的【out put file name】
    if(!ofs)
    {
        std::cerr << "ostream is not good" << endl;
        return;
    }

    for(size_t idx = 0; idx != _dict.size(); ++idx)
    {
        ofs << _dict[idx]._word << "   " << _dict[idx]._frequency << endl;
    }//存储单词与词频

    ofs.close();
}

string Dictionary::dealWord(const string &word)
{
    for(size_t idx = 0; idx != word.size(); ++idx)
    {
        //遍历所有单词
        if(!isalpha(word[idx]))//abc123
        {//快速检查是否全字母//一旦不是字母直接返回空字符//也可以在这里完成替换
            return string();//返回的是临时对象//string()的初始化应该是空白的
        }
    }

    return word;
}

void Dictionary::insert(const string &word)
{
    if(word == string())//检查符准备好，预处理不对劲直接扔
    {
        return;
    }

    size_t idx  = 0;
    for(idx = 0; idx != _dict.size(); ++idx)//开始遍历插入
    {//比较原始和花时间的方法，改进的话可以用哈希
        if(word == _dict[idx]._word) 
        {
            ++_dict[idx]._frequency;//word
            break;//当单词词频统计之后，就不需要进行循环操作了
        }
    }

    if(idx == _dict.size())//如果是新词
    {
        _dict.push_back(Record(word, 1));//插入函数，插入到词典并词频为1
        //先构造，再插入
    }

}
int main(int argc, char **argv)
{
    Dictionary dictionary(13000);
    cout << "start reading...." << endl;
    time_t beg = time(NULL);
    dictionary.read("The_Holy_Bible.txt");
    time_t end = time(NULL);
    cout << "cost " << (end - beg) << "s" << endl;
    cout << "finish reading...." << endl;
    dictionary.store("dic.dat");//这个是没有就强行开的？
    return 0;
}

