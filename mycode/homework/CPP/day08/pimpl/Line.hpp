#ifndef __WD_LINE_HPP__
#define __WD_LINE_HPP__

class Line{
public:
    Line(int x1,int y1,int x2,int y2);
    ~Line();
    void printLine()const;
private:
    class LineImpl;
    LineImpl* _pimpl;
};
#endif
