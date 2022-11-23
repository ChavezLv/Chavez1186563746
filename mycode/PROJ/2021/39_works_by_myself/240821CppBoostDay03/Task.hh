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

namespace wd
{

class Task{
public:
    virtual
    void process() = 0;
};


}//end of namespace wd

#endif //TASK_HH
