/**
 * Project 魔兽世界
 */


#ifndef _INITDATA_H
#define _INITDATA_H

#include "Warrior.h"
#include <iostream>
#include <map>
using std::map;

namespace warcraft{

// 对于颜色、武士类型，设计成枚举类型，方便以后的使用
enum Color{
    RED,
    BLUE,
    NOTSET
};

enum WarriorType{
    DRAGON_TYPE,
    NINJA_TYPE,
    ICEMAN_TYPE,
    LION_TYPE,
    WOLF_TYPE,
};

class Warrior;
using WarriorPtr = Warrior*;

auto WarriorCmp = [](WarriorPtr lhs, WarriorPtr rhs){
    return lhs->getHp() > rhs->getHp();
};

class InitData {
public: 
    size_t _initElements;
    size_t _cityCount;
    size_t _minutes;
    map<WarriorType, size_t> _initWarriorLife;
    map<WarriorType, size_t> _initAttack;
};


}// end of namespace warcraft

#endif //_INITDATA_H
