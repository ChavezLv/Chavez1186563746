#ifndef __EDITDISTANCE_HPP__
#define __EDITDISTANCE_HPP__

#include <string>

using std::string;

namespace spellCorrect
{

//1.求一个字符占据的字节数
std::size_t nBytesCode(const char ch);
//2.求一个字符串中的字符数
std::size_t charLength(const string& str);
//3.求2个字符串的最小编辑距离
int editDistance(const string& lhs,const string& rhs);

}//end of namespace spellCorrect
#endif
