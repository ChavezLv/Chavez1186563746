/* <MyResult> -*- C++ -*- */
#ifndef __MYRESULT_H__
#define __MYRESULT_H__

#include <string>

namespace wd
{

struct MyResult
{
    std::string _word; // 候选词
    int _iFreq; // 词频
    int _iDist; // 与查询词的最小编辑距离
};

}// end of namespace wd
#endif

