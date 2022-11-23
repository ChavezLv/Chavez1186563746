/**
 * Project 魔兽世界
 */


#ifndef _GAMECONFIG_H
#define _GAMECONFIG_H


#include "InitData.h"
#include <istream>
#include <string>
#include <vector>
using std::vector;
using std::string;
using std::istream;

namespace warcraft{

class GameConfig {
public: 

    void setWarriorOrder();

    void readFromConsole();

    /**
     * @param string & filename
     */
    void readFromFile(string & filename);

    size_t currentGroupId();

    /**
     * @param WarriorType
     */
    size_t warriorInitialLife(WarriorType);

    size_t headquartersInitialElements();
private: 
    vector<InitData> _groups;
    vector<WarriorType> _initWarriorOrder;
    size_t _currentGroupId;

    /**
     * @param istream&is
     */
    void readFromStream(istream&is);
};

}//end of namespace warcraft

#endif //_GAMECONFIG_H
