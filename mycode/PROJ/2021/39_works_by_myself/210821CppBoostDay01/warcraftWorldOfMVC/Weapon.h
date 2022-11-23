/**
 * Project 魔兽世界
 */


#ifndef _WEAPON_H
#define _WEAPON_H

class Weapon {
public: 
    
size_t getAttackValue();
protected: 
    string _name;
    size_t _attack;
};

#endif //_WEAPON_H