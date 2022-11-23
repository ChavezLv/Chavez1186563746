 ///
 /// @file    TestObserver.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-16 11:25:23
 ///
 
#include "Observer.hpp"
#include "Subject.hpp"

#include <memory>
#include <iostream>
using std::cout;
using std::endl;
using std::unique_ptr;
 
void test0() 
{
	unique_ptr<Observer> baby1(new Baby("gazi"));
	unique_ptr<Observer> baby2(new Baby("panda"));
	unique_ptr<Observer> nurse1(new Nurse("daiyu"));
	unique_ptr<Observer> nurse2(new Nurse("qingwen"));
	Person p("huajiang");

	Ring ring;

	ring.attach(baby1.get());
	ring.attach(baby2.get());
	ring.attach(nurse1.get());
	ring.attach(nurse2.get());

	p.knockAt(ring);

	ring.detach(nurse2.get());
	cout << endl;

	p.knockAt(ring);
} 
 
int main(void)
{
	test0();
	return 0;
}
