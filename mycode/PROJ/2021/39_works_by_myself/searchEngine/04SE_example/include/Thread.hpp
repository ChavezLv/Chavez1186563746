#ifndef __THREAD_HPP__
#define __THREAD_HPP__

#include "NonCopyable.hpp"

#include <functional>
#include <string>

using std::string;

namespace spellCorrect
{
namespace current_thread
{
extern __thread const char* name;
}
using ThreadCallback = std::function<void()>;

class Thread
    :NonCopyable
{
public:
    Thread(ThreadCallback && cb, const string& name = string());
    ~Thread();

    void start();
    void join();

    bool isRunning() const;
    string getName() const;
private:
    static void* threadFunc(void* arg);
private:
    pthread_t _pthid;
    string _name;
    bool _isRunning;
    ThreadCallback _cb;
};

}//end of namespace spellCorrect
#endif
