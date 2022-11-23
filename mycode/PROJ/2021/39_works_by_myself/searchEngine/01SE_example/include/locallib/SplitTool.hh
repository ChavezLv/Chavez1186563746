/* <SplitTool> -*- C++ -*- */
#ifndef __SPLITTOOL_H__
#define __SPLITTOOL_H__
#include "Configuration.hh"
#include <string>
#include <vector>
using std::vector;
using std::string;
namespace wd
{

class SplitTool
{
public:
    SplitTool(){}
    virtual ~SplitTool() {};
    virtual vector<string> cut(const string & sentence) = 0;

};

}// end of namespace wd
#endif

