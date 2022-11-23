#include "../../include/EditDistance.hpp"

#include <iostream>

using std::cout;
using std::endl;


namespace spellCorrect
{
std::size_t nBytesCode(const char ch)
{
    if(ch & (1 << 7))
    {
        int nBytes = 1;
        for(int idx = 0; idx != 6; ++idx)
        {
            if(ch & (1 << (6 - idx)))
            {
                ++nBytes;
            }
            else
            {
                break;
            }
        }
        return nBytes;
    }
    return 1;
}

std::size_t charLength(const string& str)
{
    std::size_t ilen = 0;
    for(int idx = 0; idx != str.size(); ++idx)
    {
        int nBytes = nBytesCode(str[idx]);
        idx += (nBytes - 1);
        ++ilen;
    }
    return ilen;
}

int editDistance(const string& lhs,const string& rhs)
{
    std::size_t llen = charLength(lhs);
    std::size_t rlen = charLength(rhs);
    int editDist[llen + 1][rlen + 1];

    for(std::size_t i = 0; i <= llen ; ++i)
    {
        editDist[i][0] = i;
    }
    for(std::size_t j = 0; j <= rlen ; ++j)
    {
        editDist[0][j] = j;
    }

    string sublhs, subrhs;
    for(std::size_t i = 1, lhs_idx = 0; i <= llen; ++i)
    {
        size_t nBytes = nBytesCode(lhs[lhs_idx]);
        sublhs = lhs.substr(lhs_idx, nBytes);
        lhs_idx += nBytes;
        for(std::size_t j = 1, rhs_idx = 0; j <= rlen; ++j)
        {
            nBytes = nBytesCode(rhs[rhs_idx]);
            subrhs = rhs.substr(rhs_idx, nBytes);
            rhs_idx += nBytes;
            if(sublhs == subrhs)
            {
                editDist[i][j] = editDist[i-1][j-1];
            }
            else
            {
                int t1 = editDist[i-1][j];
                t1 = t1 < editDist[i][j-1] ? t1 : editDist[i][j-1];
                t1 = t1 < editDist[i-1][j-1] ? t1 : editDist[i-1][j-1];
                editDist[i][j] = t1 + 1;
            }
        }
    }
    return editDist[llen][rlen];
}

}//end of namespace spellCorrect

#if 0
using namespace spellCorrect;
void test()
{
    string s1 = "abcd";

    string s2 = "中国";
    for(auto & ch : s2) {
        cout << ch;

    }
    cout << endl;
    cout << "s2[1]: " << s2[1] << endl;
    cout << endl;
    cout << "s1.size() : " << s1.size() << endl;
    cout << "s2.size() : " << s2.size() << endl;

    string s3 = s2.substr(0, 3);
    cout << "s3.size(): " << s3.size() << endl;
    cout << "s3: " << s3 << endl;

    string s4 = s2.substr(1, 3);
    cout << "s4.size(): " << s4.size() << endl;
    cout << "s4: " << s4 << endl;

    //字符的长度
    string s5 = "abc中国";
    cout << "s5.size(): " << s5.size() << endl; 
    cout << "s5.length: " << charLength(s5) << endl;
    
    // 中国   => h中国 => a中国 => ab中国 => abc中国
    // s2经过编辑之后，变成s5
    cout << "s2与s5的最小编辑距离: " << editDistance(s2, s5) << endl;
}

int main()
{
    test();
}
#endif
