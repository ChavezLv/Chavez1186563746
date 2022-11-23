#include "../../include/SplitToolCppJieba.hpp"
#include "../../include/Configuration.hpp"

#include "../../include/cppjieba/Jieba.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <map>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::pair;

namespace spellCorrect
{
const string CONF_PATH = "../conf/";
SplitToolCppJieba::SplitToolCppJieba()
    :_conf(*Configuration::getInstance(CONF_PATH))
{}


vector<string> SplitToolCppJieba::cut(const string& sentence)
{
    map<string,string>& configMap = _conf.getConfigMap();
    const char* const DICT_PATH = configMap["DICT_PATH"].c_str();
    const char* const HMM_PATH = configMap["HMM_PATH"].c_str();
    const char* const USER_DICT_PATH = configMap["USER_DICT_PATH"].c_str();
    const char* const IDF_PATH = configMap["IDF_PATH"].c_str();
    const char* const STOP_WORD_PATH = configMap["STOP_WORD_PATH"].c_str();
    cppjieba::Jieba jieba(DICT_PATH
                          ,HMM_PATH
                          ,USER_DICT_PATH
                          ,IDF_PATH
                          ,STOP_WORD_PATH);
    vector<string> words;
    jieba.CutForSearch(sentence,words);
    return words;
}

}//end of namespace spellCorrect

void testJieba()
{
    string s = "小明同学想上班";
    spellCorrect::SplitToolCppJieba jieba;
    vector<string> words = jieba.cut(s);
    for(auto& elem:words)
    {
        cout << elem << "/";
    }
    cout << endl;
}
#if 0
int main()
{
    testJieba();
    return 0;
}
#endif
