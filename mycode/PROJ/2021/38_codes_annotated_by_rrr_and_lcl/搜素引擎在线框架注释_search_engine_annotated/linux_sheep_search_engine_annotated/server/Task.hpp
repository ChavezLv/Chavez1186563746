 ///
 /// @file    Task.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-06-12 15:48:32
 ///
 
#ifndef __WD_TASK_HPP__
#define __WD_TASK_HPP__

#include <functional>
namespace wd
{

using Task = std::function<void()>;
 
}//end of namespace wd
#endif
