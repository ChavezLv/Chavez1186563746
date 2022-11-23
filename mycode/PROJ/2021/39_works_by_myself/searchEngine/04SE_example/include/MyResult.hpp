#ifndef __MYRESULT_HPP__
#define __MYRESULT_HPP__

#include <string>

namespace spellCorrect
{

struct MyResult
{
    std::string _word;
    int _iFreq;
    int _iDist;
};

}//end of namespace spellCorrect
#endif
