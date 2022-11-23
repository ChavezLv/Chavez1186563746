/**
 * Project 魔兽世界
 */


#ifndef _DRAGON_H
#define _DRAGON_H

#include "Warrior.h"

namespace warcraft{


class Dragon: public Warrior {
public:
    Dragon(Color color, size_t id, size_t hp, size_t forces, float morale)
    : Warrior(color, "dragon", id, hp, forces, DRAGON_TYPE)
    , _morale(morale){
        
    }

    virtual
    float getMorale() const{
        return _morale;
    }

private:
    float _morale;
};


}// end of namespace warcraft

#endif //_DRAGON_H
