#include <iostream>
#include <vector>
#include <algorithm>
/* using namespace std; */
using std::cout;
using std::endl;

class Rectangle {
public:
    Rectangle(float width, float length):
    m_width(width),
    m_length(length){

    }
    void  area(){
        cout << m_width * m_length << endl;
    }

    void perimeter(){
        cout << 2 * m_width + 2 * m_length << endl;
    }
private:
    float m_width;
    float m_length;
};
int main()
{
    Rectangle rec1(13.1, 5.02);
    rec1.area();
    rec1.perimeter();

    Rectangle rec2(123.4, 567.8);
    rec2.area();
    rec2.perimeter();
    return 0;
}

