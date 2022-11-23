 ///
 /// @file    Observer.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-16 11:16:38
 ///
 
#include "Observer.hpp"
#include "Subject.hpp"

#include <iostream>
using std::cout;
using std::endl;
 

void Baby::update()
{
	srand(clock());
	int number = rand() % 100;
	if(number < 70) {
		cout << "> Baby " << _name << " is sleeping sweetly!" << endl;	
	} else {
		cout << "> Baby " << _name << " is crying loudly！" << endl;
	}
}

void Nurse::update()
{
	//观察者模式中的拉模式(主动获取消息)
	/* Status * p = _psubject->getStatus(); */

	srand(clock());
	int number = rand() % 100;
	if(number < 70) {
		cout << ">> Nurse " << _name << " is ready to open the door！" << endl;
	} else {
		cout << ">> Nurse " << _name << " is sleeping sweetly!" << endl;	
	}
}

void Person::knockAt(Ring & ring)
{
	cout << ">> Person " << _name << " start koncking at the door!" << endl;
	ring.bePressed();
}
