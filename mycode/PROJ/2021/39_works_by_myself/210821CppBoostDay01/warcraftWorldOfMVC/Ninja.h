/**
 * Project 魔兽世界
 */


#ifndef _NINJA_H
#define _NINJA_H

#include "Warrior.h"

namespace warcraft{

class Ninja: public Warrior {
public: 
    Ninja(Color color, size_t id, size_t hp, size_t forces)
    : Warrior(color, "ninja", id, hp, forces, NINJA_TYPE){ 

    }

    /**
     * @param WarriorPtr
     * ninja 挨打了也从不反击敌人，需覆盖
     */
    virtual void defense( WarriorPtr) override;
};

}// end of namespace warcraft

#endif //_NINJA_H
