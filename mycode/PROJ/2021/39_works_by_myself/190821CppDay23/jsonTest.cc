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

#include <deque>
using std::deque;

#include <list>
using std::list;

#include <map>
using std::map;

#include <fstream>
using std::ifstream;

template<typename _Tp>
class Solution {
public:

};

void test01(){
    json js;
    ifstream file("json.properties");
    file >> js;

    string name;
    string phone;
    string address;
    int age;
    bool vip;
    vector<string> lan;
    vector<std::pair<string, double> > books;

    js.at("name").get_to(name);
    js.at("phone").get_to(phone);
    js.at("address").get_to(address);
    js.at("age").get_to(age);
    js.at("vip").get_to(vip);
    auto iter = js.find("languanges");
    for (size_t n = 0; iter != js.end() && n < iter->size(); n++) {
        lan.push_back((*iter)[n]);
                    
    }
    auto iter1 = js.find("books");
    for(size_t n = 0; iter != js.end() && n < iter->size(); n++){
        string bookName = (*iter1).items().begin().key();
        double bookPrice = (*iter1).items().begin().value();

        std::pair<string, double> book = make_pair(bookName, bookPrice);
        books.push_back(book);
    }

    cout << "start" << endl;
    cout << name << endl;
    cout << "end" << endl;
    cout << age << endl;
    for(auto &elem : lan){
        cout << elem << endl;
    }  
    cout << vip << endl;
    cout << address << endl;
}
int main(int argc, char* argv[])
{

    test01();
    return 0;
}

