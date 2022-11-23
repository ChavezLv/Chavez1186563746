/**
 * Project 魔兽世界
 */


#ifndef _LION_H
#define _LION_H

#include "Warrior.h"

namespace warcraft{


class Lion: public Warrior {
public:
    Lion(Color color, size_t id, size_t hp, size_t forces)
    : Warrior(color, "lion", id, hp, forces, LION_TYPE){

    }

    virtual
    size_t getLoyalty() const {
        return _loyalty;
    }

private:
    size_t _loyalty;
};

}//end o namespace warcraft

#endif //_LION_H
