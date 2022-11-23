#ifndef __SPLITTOOL_HPP__
#define __SPLITTOOL_HPP__

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace spellCorrect
{

class SplitTool
{
public:
    SplitTool() {}
    virtual ~SplitTool() {}
    virtual vector<string> cut(const string& sentence) = 0;
};

}//end of namespace spellCorrect 
#endif
