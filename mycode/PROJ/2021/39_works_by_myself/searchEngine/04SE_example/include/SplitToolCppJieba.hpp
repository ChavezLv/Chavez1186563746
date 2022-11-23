#ifndef __SPLITTOOLCPPJIEBA_HPP__
#define __SPLITTOOLCPPJIEBA_HPP__

#include "SplitTool.hpp"


namespace spellCorrect
{
class Configuration;
class SplitToolCppJieba
    :public SplitTool
{
public:
    SplitToolCppJieba();
    vector<string> cut(const string& sentence);

private:
    Configuration& _conf;//配置文件路径
};

}//enf of namespace spellCorrect

#endif
