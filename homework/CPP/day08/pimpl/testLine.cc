#include "Line.hpp"

#include <iostream>
using std::cout;
using std::endl;
 
void test0() 
{   //don`t care about that LineImpl and Point class
	Line line(1, 2, 3, 4);
	line.printLine();
	cout << "sizeof(Line): " << sizeof(Line) << endl;
} 
 
int main(void)
{
	test0();
	return 0;
}
