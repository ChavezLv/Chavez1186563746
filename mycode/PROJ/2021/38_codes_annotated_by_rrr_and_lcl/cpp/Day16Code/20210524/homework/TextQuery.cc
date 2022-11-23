#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>

using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::vector; //引进向量
using std::set;//引进set
using std::map;//引进map
using std::string;

class TextQuery
{
public:
    //构造函数先分配一定空间的大小
    TextQuery()
    {
        _file.reserve(100);//向量预留初始大小100
    }

    void readFile(const string &filename);//读文件接口
    void query(const string &word);//查询接口
private:
    void preProceccLine(string &line);//预处理接口
private:
    vector<string> _file;//每次获取一行并存起来  //string向量【存储行】

    map<string, int> _dict;//单词以及词频
    map<string, set<int>> _word2line;//单词以及所在行号(注意：同一个单词在相同行出现，只记录一次)
    //应该是这样：肯定【不是一个大set】，是【一个字符串对应一个总体的set】
};
void TextQuery::readFile(const string &filename)
{
    ifstream ifs(filename); //【读入文件流】 --->打开filename并开始读入
    if(!ifs)//ifs.fail()!=0
    {
        cout << "ifstream open " << filename << " error!" << endl;//fail()==1证明打开失败囖
        return;
    }

    string line; //（本函数临时变量）存储一行
    size_t lineNumber = 0;//存储行号
    while(getline(ifs, line))//拿到一行--->这个函数【转克格式化读取】【只管回车符】
    {
        _file.push_back(line);//读一行，并将结果存在vector中(对单词处理前就存起来，存的是原始的)
        preProceccLine(line);//对读取行的单词进行处理-->本代码预处理函数【非字母全换空格】

        istringstream iss(line);//【字符串读取流】-->拿来再专门处理字符串
        string word;//存储单词
        while(iss >> word) //读取line(istringstream对象)//并且【格式化读取】【一个词一个词往word放】
        {
            ++_dict[word];//统计单词与词频 //dict是【map】，就非常便利
            //1-红黑树的查找比vector的查找要快很多
            //2-可以自动去重+添加词频（word做索引，内容是词频）
            _word2line[word].insert(lineNumber);//将单词所在的行记录下来
            //大概懂了！！！//[R] _word2line[word]等价于【已经是一个set】，所以【这个set的插入】是【不一定成功的】
            //1-word接着做索引
            //2-行号的set,set的好处:同行多词不会重复记录
        }

        ++lineNumber;//行号++
    }

    ifs.close();//结束了记得关闭文件流
}

void TextQuery::query(const string &word)//查询
{
    int count = _dict[word];//获取单词出现的次数 //直接通过word索引拿答案
    //拿到【出现次数】//[Q]这里不存在是不是自动返回0？

    //找一个漂亮的格式输出：
    cout << word << " occurs " << count << (count > 1 ? " times" : " time.") << endl;//打印单词次数

    //auto:自动类型
    auto lines = _word2line[word];//对同一个单词出现的行进行遍历，输出单词以及行号
    //lines此时是一个【行号的map】
    for(auto &number : lines) //lines应该是一整行了
        //应该是【往number里填入】【map结合的值】
    {
        cout << "    (line " << number + 1 << ") " << _file[number] << endl;
        //输出行的输出数量
    }
}

void TextQuery::preProceccLine(string &line)//预处理：按行预处理
{
    for(auto &ch : line)//自动遍历【字符ch in line】//好像逐渐明白这种语法了
    {
        if(!isalpha(ch))//处理单词，如果不是字母就用空格代替
        {
            ch = ' ';
        }
        else if(isupper(ch))//如果是大写就转为小写
        {
            ch = tolower(ch);//顺便还多了一个大写转小写耶
        }
    }
}

int main(int argc, char **argv)
{
    TextQuery tq;//文字查询对象 //默认构造函数
    tq.readFile("china_daily.txt");//readfile,会【初步填写数据成员】
    cout << " please input a query word: " << endl;
    string word;//没有做输入检查
    while(std::cin >> word)//接住输入的单词
    {
        tq.query(word);//不过会反复重复输入的过程
    }
    return 0;
}

