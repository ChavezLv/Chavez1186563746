 ///
 /// @file    Task.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-17 11:13:24
 ///
 
#ifndef __WD_TASK_HPP__
#define __WD_TASK_HPP__

#include <functional>

namespace wd
{
using Task = std::function<void()>;
}// end of namespace wd
 
 
#endif
