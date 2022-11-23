#include "../../libs/cppjieba/Jieba.hpp"
#include "../../include/locallib/SplitToolCppJieba.hh"

namespace wd
{

SplitToolCppJieba::SplitToolCppJieba(Configuration & conf)
    : _conf(conf)
{
    /* std::cout << "cppjieba_init" << std::endl; */
}
SplitToolCppJieba::~SplitToolCppJieba()
{}

vector<string> SplitToolCppJieba::cut(const string & sentence)
{
    cppjieba::Jieba jieba((_conf.getConfigMap())["dict_path"],
                           (_conf.getConfigMap())["hmm_path"],
                           (_conf.getConfigMap())["user_dict_path"],
                           (_conf.getConfigMap())["idf_path"],
                           (_conf.getConfigMap())["stopCnPath"]);
    vector<string> jiebawords;
    jieba.CutForSearch(sentence,jiebawords,true);
    /* std::cout << limonp::Join(jiebawords.begin(), jiebawords.end(), " ") << std::endl; */
    return jiebawords;
}
}//end of namespace wd
