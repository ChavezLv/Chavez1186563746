/**
 * Project 魔兽世界
 */


#ifndef _CITY_H
#define _CITY_H

#include "InitData.h"
#include <vector>
using std::vector;

namespace warcraft{


class City {
public: 
    City(size_t id, size_t elements = 0)
    : _flag(Color::NOTSET)
    , _id(id)
    , _elements(elements)
    , _redWinCount(0)
    , _blueWinCount(0){

    }

    /**
     * @param WarriorPtr
     */
    void attach(WarriorPtr);//某武士进入该城市

    /**
     * @param WarriorPtr
     */
    void detach(WarriorPtr);//某武士离开该城市

    void startBattle();//开始战斗

    void produceElements();//城市生产生命元

    void takenByWarrior(WarriorPtr warrior);//生命元被某武士取走

    size_t getId() const { return _id; }
    Color getColor() const { return _flag; }
    size_t getWarriorAmount() const { 
        return _redWarriors.size() + _blueWarriors.size();
    }
    vector<WarriorPtr> & getRedWarriors() { return _redWarriors; }
    vector<WarriorPtr> & getBlueWarriors() { return _blueWarriors; }

private: 
    Color _flag;
    size_t _id;//
    size_t _elements;//城市生命元
    size_t _redWinCount;//红魔军连续胜利次数
    size_t _blueWinCount;
    vector<WarriorPtr> _redWarriors;
    vector<WarriorPtr> _blueWarriors;

    /**
     * @param WarriorPtr
     * @param WarriorPtr
     */
    void battle(WarriorPtr, WarriorPtr);

    /**
     * @param WarriorPtr
     */
    bool isChangingFlag(WarriorPtr);
};

}// end of namespace warcraft


#endif //_CITY_H
