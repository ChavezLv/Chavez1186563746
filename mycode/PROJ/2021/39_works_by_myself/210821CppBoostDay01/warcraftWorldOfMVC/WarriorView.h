/**
 * Project 魔兽世界
 */


#ifndef _WARRIORVIEW_H
#define _WARRIORVIEW_H

class WarriorView {
public: 
    
void showBorn();
    
virtual void showMarch() = 0;
    
void showEarnElements();
    
void showReachDestination();
protected: 
    Warrior* _warror;
};

#endif //_WARRIORVIEW_H