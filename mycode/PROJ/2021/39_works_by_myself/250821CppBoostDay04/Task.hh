/*================================================================
*   Copyright (C) 2021 WD39th-ゞゞゞ All rights reserved.
*   
*   @file       ：Task.hh
*   @author     ：WD39th-ゞゞゞ
*   @date       ：2021年08月24日 22:36:08
*   @description：
*
================================================================*/


#ifndef __WD_TASK_HPP__
#define __WD_TASK_HPP__ 

#include <iostream>
#include <functional>

namespace wd
{

using Task = std::function<void () >;//bo

/* class Task{//oo */
/* public: */
/*     virtual */
/*     void process() = 0; */
/* }; */


}//end of namespace wd

#endif //TASK_HH
