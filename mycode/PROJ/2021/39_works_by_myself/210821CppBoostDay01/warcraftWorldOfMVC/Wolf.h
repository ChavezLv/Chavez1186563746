/**
 * Project 魔兽世界
 */


#ifndef _WOLF_H
#define _WOLF_H

#include "Warrior.h"

namespace warcraft{


class Wolf: public Warrior {
public: 
    Wolf(Color color, size_t id, size_t hp, size_t forces)
    : Warrior(color, "wolf", id, hp, forces, WOLF_TYPE){

    }
    
    /*
     * 在一个wolf 通过主动攻击杀死敌人的次数达到偶数的时刻（次数从1开始
     * 算），在战斗完成后，该 wolf 生命 值和攻击力都增加1倍。如果其杀死
     * 的敌人是lion,则攻击力和生命值先加倍，然后才吸取lion的生命值。获取
     * 总部的生命元奖励，发生在加倍之后。只有在主动攻击杀死敌人时才能有加倍的
     * 事情，反击杀死敌人则不不会发 生加倍。
     */
    virtual
    void attack(WarriorPtr warrior) override{

    }
private:
    int _killTimes;
};


}//end of namespace Warcraft

#endif //_WOLF_H
