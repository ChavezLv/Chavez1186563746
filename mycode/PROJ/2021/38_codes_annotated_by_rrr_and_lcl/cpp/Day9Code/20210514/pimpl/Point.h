#ifndef __POINT_H__
#define __POINT_H__

class Line //抽象类层：Line，目标是【让用户知道可以完成一些Line类相关的操作】
{
public:
    Line(int x1, int y1, int x2, int y2);//Line类的构造函数(必须提供2点+共计4坐标)
    void printLine();//提供接口：目前只提供【打印线条】接口
    ~Line();//Line类的析构函数
    class PointPimpl;//内部公开类：在外面可以访问这个类，只是要Line::PointPimpl-->从pimpl的需求来说应该是不可以的
    //如果放在下面的private里就是【内部私有类】：仅仅只有本类可以访问此类内的数据
    //[r]其实我觉得这里放private比较好//-->确实是的，无论是pimpl的课件还是bridge的讲解都是放在private的
private:
    PointPimpl *_pimpl;//私有类唯一成员：指向具体实现类的指针 
};

#endif
