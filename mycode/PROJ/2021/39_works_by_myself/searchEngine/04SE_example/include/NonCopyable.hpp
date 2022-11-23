#ifndef __NONCOPYABLE_HPP__
#define __NONCOPYABLE_HPP__

namespace spellCorrect
{

class NonCopyable
{
protected:
    NonCopyable() {}
    ~NonCopyable() {}

    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

}//end of namespace spellCorrect

#endif
