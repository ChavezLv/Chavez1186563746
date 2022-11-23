/**
 * Project 魔兽世界
 */


#ifndef _HEADQUARTERS_H
#define _HEADQUARTERS_H

#include "InitData.h"
/* #include "Warrior.h" */
#include <vector>
#include <queue>
#include <unordered_map>
using std::priority_queue;
using std::vector;
using std::unordered_map;

namespace warcraft{


class Headquarters {
public: 

    /**
     * @param Color color
     * @param size_t element
     */
    Headquarters(Color color, size_t elements)
    : _color(color)
    , _elements(elements)
    , _earnElements(0)
    , _nextWarriorIndex(0)
    , _winners(WarriorCmp){

    }

    virtual ~Headquarters(){  }


    void setWarriorCreatingOrder();

    WarriorPtr create();//
    

    /**
     * @param WarriorPtr
     */
    void addWinner( WarriorPtr);

    void reward();

    //统计从城市获取的生命元
    void increaseElements(size_t elements){ _earnElements += elements; }
    //设置所剩生命元的数量
    void sentElements(size_t elements) { _elements = elements; }

    template<typename Iterator>
        void setWarriorCreatingOrder(Iterator beg, Iterator end){
            for(; beg != end; ++beg){
                _warriorCreatingOrder.push_back(*beg);
            }
        }

    void setId(size_t id) { _id = id; }
    size_t getId() const { return _id; }

    Color getColor() const { return _color; }
    size_t getElements() const { return _elements; }
    vector<WarriorPtr> & getWarriors() { return _warriors; }

    /**
     * @param WarriorType
     */
    size_t getWarriorAmount( WarriorType warriorType){ 
        return _warrioTypeAmounts[warriorType]; 
    }

private: 
    void nextWarriorType();

protected: 
    size_t _id;
    Color _color;
    size_t _elements;
    size_t _earnElements;//一次战斗时刻结束后，所有武士获取的生命
    size_t _nextWarriorIndex;//下次生产的武士下标
    vector<WarriorType> _warriorCreatingOrder;
    vector<Warrior*> _warriors;//生产的所有武士
    unordered_map<WarriorType, size_t> _warrioTypeAmounts;//各种类型武士的数量
    priority_queue<WarriorPtr, vector<WarriorPtr>, decltype(WarriorCmp)> _winners;//一次战斗时间结束后，胜利的所有武士

};

}// end of namespace warcraft

#endif //_HEADQUARTERS_H
