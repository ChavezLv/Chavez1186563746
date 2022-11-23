/**
 * Project 魔兽世界
 */


#ifndef _WARRIOR_H
#define _WARRIOR_H

#include "InitData.h"
#include "Headquarters.h"
#include <string>
using std::string;

namespace warcraft{

class Warrior {
public: 

    /**
     * @param color
     * @param id
     * @param hp
     * @param weapon
     */
    Warrior(Color color, const string & name, size_t id, size_t hp, size_t forces, WarriorType weapon);

    virtual
    ~Warrior(){  }

    virtual
    void march();//行军

    /**
     * @param WarriorPtr
     */
    virtual
    void attack(WarriorPtr);//攻击

    /**
     * @param WarriorPtr
     */
    virtual
    void defense(WarriorPtr);//反击

    void berewarded();//被奖励

    void beWinner();//成为胜利者

    void sentElementsToHeadquarters(size_t elements);//为司令部获取生命元

    void setHp(size_t hp) { _hp = hp; }
    void setCity(size_t id) { _cityId = id; }
    void setHeadquarters(Headquarters *headquarters) {
        _headquarters = headquarters;
    }



    size_t getId(){ return _id; }
    size_t getHp(){ return _hp; }
    size_t getForces(){ return _forces; }
    size_t getCityId() const{ return _cityId; }
    size_t getElements() const{ return _hp; }
    size_t getEarnElements() const { return _earnElements; }
    string getName() { return _name; }
    Color getColor() const { return _color; }
    WarriorType getType() const{ return _type; }

protected: 
    Color _color;
    string _name;
    size_t _id;//编号
    size_t _hp;//生命值
    size_t _forces;
    size_t _cityId;//位于某个城市
    size_t _earnElements;//从某个城市中获取的生命元
    WarriorType _type;
    Headquarters* _headquarters;
};



}// end of namespace warcraft

#endif //_WARRIOR_H
