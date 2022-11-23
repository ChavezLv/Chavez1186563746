#include "Configuration.h"

#include <vector>

using std::vector;
using std::ofstream;

namespace yt
{

class DictProducer
    /* :public Configuration */
{
public:
    DictProducer(const string &endir,const string &cndir,const string &aimsdir)
        :_endir(endir)
         ,_cndir(cndir)
         ,_aimsdir(aimsdir)
    {
        getenfile();
        /* cout << "--------" <<endl; */
        getcnfile();
    }    

    /* DictProducer(const string &dir,SplitTool *splitTool); */

    //创建英文字典
    void build_dict();

    //创建中文字典
    void build_cn_dict();

    //将词典写入文件
    void store_dict();

    //查看词典路径，作为测试用
    void  show_files() const;

    //查看文件路径，作为测试用
    void show_dict() const;

    //获取文件的绝对路径
    void get_enfiles();

    //存储某个单词
    void push_dict(const string &word);

    //获取英文语料库配置文件中所有路径
    void getenfile();

    //获取中文语料库配置文件中所有路径
    void getcnfile();

    //调整英文单词
    void adjustmentEnglish(string &word);

    //调整中文单词
    void adjustmentChinese(string &word);

private:
    string _cndir;  //中文语料库配置地址
    string _endir;  //英语语料库配置地址
    string _aimsdir; //目标词典地址
    vector<string> _cnfiles;    //中文语料库路径集合
    vector<string> _enfiles;    //英文语料库路径集合
    map<string,int> _dict;
};

}//end of namespace yt
