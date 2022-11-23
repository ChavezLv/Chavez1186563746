#ifndef __SPLITTOOLCPPJIEBA_H__
#define __SPLITTOOLCPPJIEBA_H__
#pragma once
#include "../../libs/cppjieba/Jieba.hpp"
#include "SplitTool.hh"
namespace wd
{

class SplitToolCppJieba
    :public SplitTool
{
public:
    SplitToolCppJieba(Configuration & conf);
    ~SplitToolCppJieba();
    vector<string> cut(const string & sentence);

private:
    Configuration & _conf;
};
}//end of namespace wd
#endif
