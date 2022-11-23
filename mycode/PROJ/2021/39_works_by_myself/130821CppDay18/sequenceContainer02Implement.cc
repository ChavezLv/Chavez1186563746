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

template<typename Container>
void display(const Container &ctn){
    for(auto &elem : ctn){
        cout << elem << " ";
    }
    cout << endl;
}
void test01(){
    cout << "<1>" << endl;       
    vector<int> nums1;
    display(nums1);

    cout << "<2>" << endl;       
    vector<int> nums2(7, 0x0814);
    display(nums2);

    cout << "<3>" << endl;       
    vector<int>::const_iterator it1 = nums2.cbegin(); 
    vector<int>::const_iterator it2 = nums2.end(); 
    vector<int> nums3(it1, it2);
    display(nums3);

    cout << "<4>" << endl;       
    vector<int> nums4(nums3);
    display(nums4);

    cout << "<5>" << endl;       
    vector<int> nums5(vector<int>(7,0x1000));
    display(nums5);

    cout << "<6>" << endl;       
    vector<int> nums6 = {0x000f, 0x00ff, 0x0fff, 0xffff};
    display(nums6);
}
void test02(){
    cout << "<1>" << endl;       
    deque<int> nums1;
    display(nums1);

    cout << "<2>" << endl;       
    deque<int> nums2(7, 0x0814);
    display(nums2);

    cout << "<3>" << endl;       
    deque<int>::const_iterator it1 = nums2.cbegin(); 
    deque<int>::const_iterator it2 = nums2.end(); 
    deque<int> nums3(it1, it2);
    display(nums3);

    cout << "<4>" << endl;       
    deque<int> nums4(nums3);
    display(nums4);

    cout << "<5>" << endl;       
    deque<int> nums5(deque<int>(7,0x1000));
    display(nums5);

    cout << "<6>" << endl;       
    deque<int> nums6 = {0x000f, 0x00ff, 0x0fff, 0xffff};
    display(nums6);
}
void test03(){
    cout << "<1>" << endl;       
    list<int> nums1;
    display(nums1);

    cout << "<2>" << endl;       
    list<int> nums2(7, 0x0814);
    display(nums2);

    cout << "<3>" << endl;       
    list<int>::const_iterator it1 = nums2.cbegin(); 
    list<int>::const_iterator it2 = nums2.end(); 
    list<int> nums3(it1, it2);
    display(nums3);

    cout << "<4>" << endl;       
    list<int> nums4(nums3);
    display(nums4);

    cout << "<5>" << endl;       
    list<int> nums5(list<int>(7,0x1000));
    display(nums5);

    cout << "<6>" << endl;       
    list<int> nums6 = {0x000f, 0x00ff, 0x0fff, 0xffff};
    display(nums6);
}
int main(int argc, char* argv[])
{

    test01();
    test02();
    test03();
    return 0;
}

