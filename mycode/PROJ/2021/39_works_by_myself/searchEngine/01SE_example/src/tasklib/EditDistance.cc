#include "../../include/tasklib/EditDistance.hh"

namespace wd
{

size_t nBytesCode(const char ch)
{
    if(ch & (1 << 7)) {
        int nBytes = 1;
        for(int idx = 0; idx != 6; ++idx) {
            if(ch & (1 << (6 - idx))) {
                ++nBytes;
            } else {
                break;
            }
        }
        return nBytes;
    }
    return 1;
}

size_t length(const std::string & str)
{
    size_t len = 0;
    for(size_t idx = 0; idx != str.size(); ++idx) {
        int nBytes = nBytesCode(str[idx]);
        idx += nBytes - 1;
        ++len;
    }
    return len;
}

}// end of namespace wd
