#include "Line.hpp"
#include <vector>
#include <iostream>
using std::cout;
using std::endl;
using std::vector;
using std::ostream;
//LineImpl类的定义
class Line::LineImpl{
    //point嵌套类
    class Point{
    public:
        Point(int ix=0,int iy=0)
            :_ix(ix)
            ,_iy(iy)
        {
            cout<<"Point(int,int)"<<endl;
        }
    ~Point(){cout<<"~point()"<<endl;}
    void print()const{
        cout<<"("<<_ix<<","<<_iy<<")";
    }
    private:
        int _ix;
        int _iy;
    };
public:
    LineImpl(int x1,int y1,int x2,int y2)
        :_p1(x1,y1)
        ,_p2(x2,y2)
    {
        cout<<"LineImpl(int,int,int ,int)"<<endl;
    }
    void printLine()const{
        _p1.print();
		cout << "--->";
		_p2.print();
		cout << endl;
    }
    ~LineImpl(){cout<<"LineImpl()"<<endl;}
private:
    Point _p1,_p2;
};

//Line类的构造与析构函数
Line::Line(int x1, int y1, int x2, int y2)
: _pimpl(new LineImpl(x1, y1, x2, y2))
{	cout << "Line(int,int,int,int)" << endl;	}
Line::~Line(){
    if(_pimpl){
        delete _pimpl;
        _pimpl=nullptr;
    }
}
void Line::printLine()const{
    _pimpl->printLine();
}
