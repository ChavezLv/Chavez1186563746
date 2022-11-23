/**
 * Project 魔兽世界
 */


#ifndef _WARCRAFTWORLD_H
#define _WARCRAFTWORLD_H

class WarcraftWorld {
public: 
    
void WarcraftWorld();
    
void setRedWarriorCreatingOrder();
    
void setBlueWarriorCreatingOrder();
    
void start();
private: 
    Headquarters * _red;
    Headquarters* _blue;
    
void init();
    
/**
 * @param Headquarters*
 */
void createWarrior(void Headquarters*);
    
bool warriorMarch();
    
void cityProduceElements();
    
void takeCityElements();
    
void battle();
    
void headquartersReportElements();
};

#endif //_WARCRAFTWORLD_H