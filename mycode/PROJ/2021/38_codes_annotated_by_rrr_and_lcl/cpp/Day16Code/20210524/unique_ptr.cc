#include <iostream>
#include <memory>
#include <vector>

using std::cout;
using std::endl;
using std::unique_ptr;
using std::vector;

class Point //点类一切照旧
{
public:
    Point(int ix = 0, int iy = 0)
    : _ix(ix)
    , _iy(iy)
    {
        cout << "Point(int = 0, int = 0)" << endl;
    }

    void print()
    {
        cout << "_ix = " << _ix << endl
             << "_iy = " << _iy << endl;
    }
    ~Point()
    {
        cout << "~Point()" << endl;
    }
private:
    int _ix;//私有变量
    int _iy;
};

void test()//突然有点理解智能指针：【正常的符号承担正常指针的一切职能】+【新增了一些位置放函数】
{
    //原生指针（裸指针）
    int *pInt = new int(10); //裸指针
    unique_ptr<int> up(pInt); //裸指针初始化up
    cout << "*up = " << *up << endl; //解引用（同普通指针）
    cout << "up.get() = " << up.get() << endl; //拿出里面的裸指针
    cout << "pInt = " << pInt << endl;//？？这个是直接打印地址吗--->待会儿看看

    cout << endl;
    /* unique_ptr<int> up2(up);//error,不支持拷贝操作 */

    cout << endl;
    unique_ptr<int> up3(new int(20)); //unique指针：1-裸指针初始化 2-哟定于一初始化
    /* up3 = up;//error,不允许赋值操作 */

    cout << endl;
    unique_ptr<Point> up4(new Point(10, 20)); //unique指针

    /*补充：vector的机制：
        verctor <T>时候填的这个T类；
        其实，vector是push_back，是【要借助这个T类的构造函数】的
    */

    vector<unique_ptr<Point>> vec; //vec 的基础类型是【unique_ptr】->Point
    /* vec.push_back(up4);//error *///不支持【拷贝语义】
    //push_back应该也有【移动语义】和【拷贝语义】 
    vec.push_back(std::move(up4)); //但是【你想要初始化一个】unique_ptr的话【支持移动语义】-->填入右值
    vec.push_back(unique_ptr<Point>(new Point(1, 3)));


    cout << endl;
    Point *pt = new Point(1, 2);
    vector<Point *> number;//不建议使用裸指针
    /* number.push_back(new Point(1, 2)); */
    number.push_back(pt);
    delete pt;

    //笔记by lcl:\
    1) 不允许赋值和拷贝\
    上述"unique_ptr<int> up2(up);"和"up3 = up" 都是错误的\
    2) \
    unique_ptr<int> up4(new int(30));\
    vector里面放 unique_ptr<int> nums;\
    nums.push_back(up4); //error\
    nums.push_back(std::move(up4));//ok, 左值变成右值\
    nums.push_back(unique_ptr<int>(new int(30)));//ok\
    3) 上述与vector<int*> vec; \
             vec.push_back(new int(10));\
             的区别? 前者不用管销毁, 后者堆空间new int(10)无法释放\
    4) unique_ptr对于某一块堆空间之能独一拥有, 因此有了shared_ptr<>

}
int main(int argc, char **argv)
{
    test();
    return 0;
}

