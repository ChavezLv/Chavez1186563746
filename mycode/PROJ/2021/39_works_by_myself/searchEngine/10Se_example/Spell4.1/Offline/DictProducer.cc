#include "DictProducer.hh"
#include "cppjieba/Jieba.hpp"

const char* const DICT_PATH = "./dict/jieba.dict.utf8";
const char* const HMM_PATH = "./dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "./dict/user.dict.utf8";
const char* const IDF_PATH = "./dict/idf.utf8";
const char* const STOP_WORD_PATH = "./dict/stop_words.utf8";

namespace yt
{
//创建英文字典
void DictProducer::build_dict()
{
    cout << "创建英文字典" << endl;
    for(auto &filepath:_enfiles)
    {
        ifstream ifs(filepath);
        if(!ifs)
        {
            cout << "英文语料库路径出错" << endl;
        }
        string word;
        while(ifs >> word)
        {
            adjustmentEnglish(word);
            if(_dict.find(word) == _dict.end())
            {   
                _dict[word] = 1;
            }
            ++_dict[word];
        }
        ifs.close();
    }
}

//创建中文字典
void DictProducer::build_cn_dict()
{
    cout << "创建中文字典" << endl;
    cppjieba::Jieba jieba(DICT_PATH,
                          HMM_PATH,
                          USER_DICT_PATH,
                          IDF_PATH,
                          STOP_WORD_PATH);
    vector<string> words;
    for(auto &filepath:_cnfiles)
    {
        ifstream ifs(filepath,std::ios::ate);
        if(!ifs)
        {
            cout << "中文语料库路径出错" << endl;
        }
        size_t length = ifs.tellg();
        char * buff = new char[length + 1]();
        ifs.seekg(0);
        ifs.read(buff, length);
        string content(buff);
        jieba.CutForSearch(content, words);
        vector<string> result;
        for(auto &word:words)
        {
            adjustmentChinese(word);
            if(_dict.find(word) == _dict.end())
            {   
                _dict[word] = 1;
            }
            ++_dict[word];
        }
    }
}

//将词典写入文件
void DictProducer::store_dict()
{
    ofstream ofs(_aimsdir);
    if(!ofs.good())
    {
        cout << "目标词典路径出错" << endl;
    }
    else
    {
        cout << "我要将词典存到文件中啦" << endl;
        for(auto buf = _dict.begin() ; buf!=_dict.end(); ++buf)
        {
            ofs << buf->first << " " << buf->second << endl; 
        }
        ofs.close();
    }
}

//查看词典路径，作为测试用
void DictProducer::show_files() const
{    
    cout << "show_files" <<endl;
    cout <<_endir << endl;
    cout << _cndir << endl;
}

//查看文件路径，作为测试用
void DictProducer::show_dict() const
{
    for (auto buf = _dict.begin();buf!=_dict.end();++buf)
    {
        cout << buf->first << " " << buf->second <<endl;
    }
}

//存储某个单词
void DictProducer::push_dict(const string &word)
{
    if(_dict.find(word) == _dict.end())
    {
        cout << "没有这个单词" << endl;
        _dict[word] = 1;
    }
    else
    {
        ++_dict[word];
    }
}

//获取英文语料库配置文件中所有路径
void DictProducer::getenfile()
{
    ifstream ifs(_endir);
    if(!ifs)
    {
        cout << "英文语料库配置路径出错" << endl;
    }
    string line;
    while(getline(ifs,line))
    {   
        _enfiles.push_back(line);
    }
    ifs.close();
}


//获取中文语料库配置文件中所有路径
void DictProducer::getcnfile()
{
    ifstream ifs(_cndir);
    if(!ifs)
    {
        cout << "中文语料库配置路径出错" << endl;
    }
    string line;
    while(getline(ifs,line))
    {   
        _cnfiles.push_back(line);
    }
    ifs.close();
}

//调整英文单词
void DictProducer::adjustmentEnglish(string &word)
{
    for(auto cit = word.begin();cit!=word.end();++cit)
    {
        if(!isalpha(*cit))
        {
            word.erase(cit);
            --cit;
        }
        else if(isupper(*cit))
        {
            *cit =tolower(*cit);
        }
    }
}

//调整中文单词
void DictProducer::adjustmentChinese(string &word)
{
    for(auto cit = word.begin();cit!=word.end();++cit)
    {
        if(isalnum(*cit))
        {
            word.erase(cit);
            --cit;
        }
    }
}


}//end of namespace yt
