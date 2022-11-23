/**
 * Project 魔兽世界
 */


#ifndef _WEAPONVIEW_H
#define _WEAPONVIEW_H

class WeaponView {
public: 
    
virtual void show() = 0;
protected: 
    Weapon* _weapon;
};

#endif //_WEAPONVIEW_H