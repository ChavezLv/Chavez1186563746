#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <unordered_set>
#include <deque>
using namespace std;

/* template<typename _Tp> */
/* static const auto io_sync_off = []() */
static const auto io_sync_off = []()
{
    // turn off sync
    std::ios::sync_with_stdio(false);
    // untie in/out streams
    std::cin.tie(nullptr);
    return nullptr;
}();
void *func(){
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    return nullptr;
}
class Solution {
public:


    //'auto' not allowed in non-static class member
    /* auto func0 = [&]{ */
    /*     cout << "xxx"; */
    /* }; */
    static constexpr auto func1 = [](){  };
    /* static const auto func1 = [](){  }; //error*/

    auto func2(int a, int b){
        /* io_sync_off1(); */
        return a + b;
    }

    auto func3(int a, int b);

    static constexpr auto io_sync_off1 = [](){
        // turn off sync
        std::ios::sync_with_stdio(false);
        // untie in/out streams
        std::cin.tie(nullptr);
        return nullptr;
    };

    /* static auto _a; */
    /* static auto _c = 0;//error: non-const static data member must be initialized out of line */
    const static auto _g = 0;
    static const auto _c = 0;

    constexpr static int  _a{20};
    /* constexpr static int  _a = 20;//{20}; */
    static int _b;
    constexpr static int & _x = _b;//关于c++：如何初始化constexpr引用
    /* const int & _y; */
    static const int &  _y;//non-const static data member must be initialized out of line
    const int& _z;
    int _bb;
    Solution()
    : _z(_a){//_z(_a)会在打印时出错
        cout << _z << endl;
    }
    /* const int &  _z = _a; */

};

/* int Solution::_a = 1; */
int Solution:: _b = 200;
const int & Solution::_y = Solution::_b;
/* constexpr int &Solution::_x = Solution::_b; */ 

static const auto io_sync_off1 = []()
{
    // turn off sync
    std::ios::sync_with_stdio(false);
    // untie in/out streams
    std::cin.tie(nullptr);
    return nullptr;
}();

#if 3
/*declared + type = decltype类型说明符*/
template<typename T, typename U>
/* decltype(t + u) add(T t, U u) {//error:use of undeclared identifier 't' */
auto add1(T t, U u) {//对
    // t和u尚未定义
    return t + u;
}
//auto和decltype一般配合使用在推导函数返回值的类型问题上
template<typename T, typename U>
auto add(T t, U u) -> decltype(t + u) {
    return t + u;
}
int a = 1, b = 2;
decltype(add(a, b)) c = add(3, 4);//decltype指定c的类型

#endif


void test01(){
    func();
    cout << c << endl;
    /* Solution::_a = 10;//cannot assign to variable '_a' with const-qualified type 'const int' */
    /* Solution::_y = 10;//cannot assign to variable '_y' with const-qualified type 'const int &' */
    /* Solution()._z = 10;//cannot assign to non-static data member '_z' with const-qualified type 'const int &' */
    cout << Solution::_a << endl;       
    cout << Solution::_b << endl;       
    cout << Solution::_x << endl;       
    cout << Solution::_y << endl;       
    cout << sizeof(string) << endl;
    /* cout << Solution()._z << endl; */       
    unordered_set<int> s;
    for(int start = 1; start < 3; ++start){
        s.insert(start);
        /* cout << s.bucket_count() << endl; */
        cout << s.load_factor() << endl;
        cout << s.max_load_factor() << endl;

    }
        for(int start = 0; start < s.bucket_count(); ++start){
            cout << s.bucket(start) << " " << s.bucket_size(start) << endl;
        }
    cout << s.size() << endl;
    for(int start = 3; start < 5; ++start){
        s.insert(start);
        cout << "1: " << (s.size() / (float)s.bucket_count()) << endl;
        cout << s.load_factor() << endl;
        cout << s.max_load_factor() << endl;

    }
        for(int start = 0; start < s.bucket_count(); ++start){
            cout << s.bucket(start) << " " << s.bucket_size(start) << endl;
        }
    vector<int> vc;
    cout << "size() of : " << vc.size() << endl;
    cout << "size of vector: " << sizeof(vc) << endl;
    deque<int> dq;
    cout << "size() of : " << dq.size() << endl;
    cout << "size of deque: " << sizeof(dq) << endl;
}

int main(int argc, char* argv[])
{
    test01();
    return 0;
}
