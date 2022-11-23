#include "json.hpp"
using namespace nlohmann;
using json = nlohmann::json;
#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <algorithm>

template<typename _Tp>
class Solution {
public:
   
};
void test01(){
    json j{
        { "name", "Judd Trump"},
            { "credits", 1754500 },
            { "ranking", 1}
    };

    std::cout << j << endl;

    j["name"] = "Ding Junhui"; //j.at("name") = "Ding Junhui";
        std::cout << j << endl;       
}
int main(int argc, char* argv[]){
    test01();
    return 0;

}
