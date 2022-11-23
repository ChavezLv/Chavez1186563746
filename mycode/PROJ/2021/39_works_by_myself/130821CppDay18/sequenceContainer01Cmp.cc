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

template<typename _Tp>
class Solution {
public:

};
class Point{
    
};
bool operator==(const Point&lhs, const Point&rhs){
    return true;
}
bool operator<(const Point&lhs, const Point&rhs){
    /* return false; */
    return true;
}
// 容器之间的比较是自定义类型时需要重载运算符
void test01(){
    vector<Point> vec1;
    vector<Point> vec2;
    if(vec1 < vec2){
        cout <<"<" << endl;
    }else if(vec1 == vec2){
        cout << "=" << endl;//
    }else{
        cout << ">" << endl;
    }


    vector<string> vec3(1, "ad");
    vector<string> vec4(1, "cb");
    if(vec3 < vec4){
        cout <<"<" << endl;//
    }else if(vec3 == vec4){
        cout << "=" << endl;
    }else{
        cout << ">" << endl;
    }
    
    vector<int> vec5 = {2, 2, 3};
    vector<int> vec6 = {2, 2, 2};
    if(vec5 < vec6){
        cout <<"<" << endl;
    }else if(vec5 == vec6){
        cout << "=" << endl;
    }else{
        cout << ">" << endl;//
    }
}
void test02(){
    deque<int> vec1;
    deque<int> vec2;
    if(vec1 < vec2){
        cout <<"<" << endl;
    }else if(vec1 == vec2){
        cout << "=" << endl;//
    }else{
        cout << ">" << endl;
    }


    deque<int> vec3(1, 2);
    deque<int> vec4(1, 3);
    if(vec3 < vec4){
        cout <<"<" << endl;//
    }else if(vec3 == vec4){
        cout << "=" << endl;
    }else{
        cout << ">" << endl;
    }
    
    deque<int> vec5 = {2, 2, 3};
    deque<int> vec6 = {2, 2, 2};
    if(vec5 < vec6){
        cout <<"<" << endl;
    }else if(vec5 == vec6){
        cout << "=" << endl;
    }else{
        cout << ">" << endl;//
    }
}
void test03(){
    list<int> vec1;
    list<int> vec2;
    if(vec1 < vec2){
        cout <<"<" << endl;
    }else if(vec1 == vec2){
        cout << "=" << endl;//
    }else{
        cout << ">" << endl;
    }


    list<int> vec3(1, 2);
    list<int> vec4(1, 3);
    if(vec3 < vec4){
        cout <<"<" << endl;//
    }else if(vec3 == vec4){
        cout << "=" << endl;
    }else{
        cout << ">" << endl;
    }
    
    list<int> vec5 = {2, 2, 3};
    list<int> vec6 = {2, 2, 2};
    if(vec5 < vec6){
        cout <<"<" << endl;
    }else if(vec5 == vec6){
        cout << "=" << endl;
    }else{
        cout << ">" << endl;//
    }
}
int main(int argc, char* argv[])
{

    test01();
    test02();
    test03();
    return 0;
}

