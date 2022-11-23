/**
 * Project 魔兽世界
 */


#ifndef _GAMETIME_H
#define _GAMETIME_H

#include <iostream>
namespace warcraft{

class GameTime {
public: 

    /**
     * @param size_t init
     */
    void Time(size_t init);

    void updateTime();

    size_t getTime();

    void showTime();
private: 
    size_t _hour;
    size_t _minute;
};


}// end of namespace warcraft

#endif //_GAMETIME_H
