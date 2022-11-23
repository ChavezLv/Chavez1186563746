/**
 * Project 魔兽世界
 */


#ifndef _ICEMAN_H
#define _ICEMAN_H

#include "Warrior.h"


namespace warcraft{

class Iceman: public Warrior {
public: 
    Iceman(Color color, size_t id, size_t hp, size_t forces)
    : Warrior(color, "iceman", id, hp, forces, ICEMAN_TYPE){ 

    }
    //iceman 每前进两步，在第2步完成的时候，生命值会减少9，攻击力会
    //增加20。
    ////但是若生命值减9后会小于等于0，则生命值不减9,而是变为1。即
    //iceman不会因走多了而死。
    virtual void march() override;
private:
    size_t _steps;
};


}//end of namespace warcraft

#endif //_ICEMAN_H
