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

#include <iterator>
using std::istream_iterator;
using std::ostream_iterator;

template<typename _Tp>
class Solution {
public:

};
void test01(){
    vector<int> nums;    
    istream_iterator<int> isi(std::cin);

    copy(isi, istream_iterator<int>(), std::back_inserter(nums));

    copy(nums.begin(), nums.end(), ostream_iterator<int>(cout, " "));
}
int main(int argc, char* argv[])
{

    test01();
    return 0;
}

