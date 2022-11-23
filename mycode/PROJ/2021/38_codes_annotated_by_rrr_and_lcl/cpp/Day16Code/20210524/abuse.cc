#include <iostream>
#include <memory>

using std::cout;
using std::endl;
using std::unique_ptr;
using std::shared_ptr;
//[R]唯一难点+不太理解的地方：关于【shared_from_this的使用】
class Point 
: public std::enable_shared_from_this<Point> //继承enable_shared_from_this，因为【我想取里面的一个成员】
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
//[R]难点解读：为什么要用shared_from_this
    /* Point *addPoint(Point *pt) */

    //1-最初版本【返回】【Pointer】(是<地址的值拷贝>)没问题
    //--->为什么没问题？因为【对裸指针的创建临时变量+拷贝操作】【不会引起<空间的被释放>】等实际影响，但是【智能指针会】

    //在外面的是这句：
    // shared_ptr<Point> sp3(sp1->addPoint(  sp2.get()/*拿到sp2的裸指针*/  ) /*调用sp1的add_pointer*/);

    shared_ptr<Point> addPoint(Point *pt)
    //操作目标：把【point(this)】的值和【point(传入的裸指针)】的值，相加，且【把this指针作为shared_ptr返回】
    //为什么【非要作为shared_ptr返回】？--->因为【只有shared_ptr】【互相之间操作】，才能【正确的完成】【对智能指针
    //指向内容的正确控制】
    {
        _ix += pt->_ix;
        _iy += pt->_iy;

        /* return this; */
        //问题所在：
        //1-首先，this到shared_ptr根本无法隐式转换
        //2-即使可以隐式转换，也有原则【不要混用裸指针和shared_ptr】,因为【你传出去的裸指针】【新建的shared_ptr】
        //和原来的shared_ptr没有关系，属于【1裸指针分别指派2share_ptr】的【十分不被推荐行为】

        /* return shared_ptr<Point>(this); */
        //问题所在：
        //1-发生了非常致命的【用(一个也许已经被shared_ptr托管的)裸指针(裸着)赋值给】【其他的shared_ptr】
        //2-而且【此处的shared_ptr】是【临时变量】，比如只有一个临时shared_ptr指向的情况(不知道有外部指向,假设没有),
        //【利用这个this裸指针】【声明出来】以后【临时变量消亡】【引用计数归零】
        //  会【顺便清理掉内存里的内容】-->那还回传个*，肯定不想这样啊
        
        /* return std::enable_shared_from_this<Point>::shared_from_this(); */
        //3-最后方案：回传一个【专门解决这个问题的】指针shared_from_this
        //它是【this指针的】【shared_ptr】(且【估计是用了weak_ptr】，可以【在外面顺利赋值另一个shared_ptr完毕】且不消亡）
        //[Q]有一处问漏了，就算这样，这个shared_ptr还是和sp1本来属于的不一个体系吧？-->
        //并不是，是【一个体系】的，因为【在外面是借助sp调用的addpointer
        //这个函数】，你是【有办法知道原来属于的指针sp的】
        return shared_from_this();//这个可以返回【一个自己的拷贝】(且关联着自己原来的sp指针)，达成【正确的copy】
    }
    ~Point()
    {
        cout << "~Point()" << endl;
    }
private:
    int _ix;
    int _iy;
};
//笔记by lcl: 智能指针错误使用情形\
test() : double free   \
\
test3() : 问题和test()相同,up1和up2都指向了同一块堆空间, 比test()更隐蔽一点\
\
test2() : 问题同上, 但是写成shared_ptr<Point> sp2(sp1);//就可以\
\
初级错误看完了, 接下来再看下中级错误\
//test4()\
shared_ptr<Point> sp3(    sp1->addPoint(     sp2.get()    )    );\
Point *add(Point *pt){\
    ...\
    return this;\
}\
sp3用了一个值初始化, sp1调用了一个add函数, 返回的是this(Point* const this)裸指针\
相当于同一块堆空间即用了sp1托管, 也用了sp3进行了托管, 只是上面的值多了个sp2\
所以返回值改为return shared_ptr<Point>(this);函数返回值也改下, 但是注意!\
此时是拷贝, 还是有sp1和return两个东西托管了, 临时对象托管了this, 所以接下来还要\
改一下, 不能用临时对象(shared_ptr)托管this\
//\
class Point \
: public std::enable_shared_from_this<Point>//公有继承\
想要用类里面的成员函数, 除了友元, 还可以用上一行的继承. 改为友元要改一下库,麻烦,这个简单\
现在就可以直接访问return shared_from_this();这样就没有任何问题了!

//接下来, 讲一下作业


void test()
{
    //不要使用不同的智能指针去托管同一个裸指针
    Point *pt = new Point(1, 2);
    unique_ptr<Point> up1(pt);
    unique_ptr<Point> up2(pt);
}
 
void test2()
{
    //不要使用不同的智能指针去托管同一个裸指针: 好的习惯就是不要留一个裸指针的左值, 直接交给智能指针托管
    Point *pt = new Point(1, 2);
    shared_ptr<Point> sp1(pt);
    shared_ptr<Point> sp2(pt);
}

void test3()
{
    //不要使用不同的智能指针去托管同一个裸指针
    unique_ptr<Point> up1(new Point(1, 2));
    unique_ptr<Point> up2(new Point(3, 4));
    up1.reset(up2.get());
}

void test4()
{
    //结合主函数看看：
    shared_ptr<Point> sp1(new Point(1, 2));//sp1指针，托管(1,2)
    cout << "sp1 = ";
    sp1->print();

    cout << endl;
    shared_ptr<Point> sp2(new Point(3, 4));//sp2指针 ，托管(3,4)
    cout << "sp2 = ";
    sp2->print();

    cout << endl;
    shared_ptr<Point> sp3(    sp1->addPoint(     sp2.get()    )    ); //拿到【sp2的裸指针】(为了【在内部取到数据计算】)
    //sp1【加完了】，返回【基于自己原来的的sp1】，可以【初始化sp3】的【一个sp拷贝】，从而最后达成了
    //【sp3】和【sp1】现在【共享指向一块空间】的效果
    cout << "sp3 = ";
    sp3->print();
}
int main(int argc, char **argv)
{
    test4();
    return 0;
}

