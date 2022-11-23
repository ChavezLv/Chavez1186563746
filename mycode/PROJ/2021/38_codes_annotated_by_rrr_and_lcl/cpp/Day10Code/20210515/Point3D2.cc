#include <iostream>

using std::cout;
using std::endl;

class Point
{
public:
    Point(int ix = 0, int iy = 0)
    : _ix(ix)
    , _iy(iy)
    {
        cout << "Point(int = 0, int = 0)" << endl;
    }

    int getY() const
    {
        return _iy;
    }

    ~Point()
    {
        cout << "~Point()" << endl;
    }
protected:
    int _ix;
private:
    int _iy;
};

class Point3D 
: protected Point
{
public:
    Point3D(int ix = 0, int iy = 0, int iz = 0)
    : Point(ix, iy)
    , _iz(iz)
    {
        cout << "Point3D(int = 0, int = 0, int = 0)" << endl;
    }

    void print() 
    {
        cout << "_ix = " << _ix << endl//protected
             << "_iy = " << getY() << endl //protected
             /* << "_iy = " << _iy << endl//error,基类的私有成员不能在类的外面进行访问 */
             << "_iz = " << _iz << endl;
    }

    ~Point3D()
    {
        cout << "~Point3D()" << endl;
    }
private:
    int _iz;
};

class Point4D 
: protected Point3D
{
public:
    void show()
    {
        cout << "_ix = " << _ix << endl//protected
             << "_iy = " << getY() << endl
             /* << "_iy = " << _iy << endl//不能访问 */
             /* << "_iz = " << _iz << endl */
             << "_im = " << _im << endl;
            
    }
    
private:
    int _im;
};
int main(int argc, char **argv)
{
    /* Point3D pt3d(1, 2, 3); */
    /* pt3d._ix = 100;//error */
    /* pt3d.getY();//error */
    return 0;
}

