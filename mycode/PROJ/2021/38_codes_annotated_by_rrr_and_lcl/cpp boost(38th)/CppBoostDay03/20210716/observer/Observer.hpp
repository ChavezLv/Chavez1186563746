 ///
 /// @file    Observer.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-16 10:58:20
 ///
 
#ifndef __OBSERVER_HPP__
#define __OBSERVER_HPP__

#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

class Ring;
class Subject;
struct Status;

class Observer
{
public:
	virtual void update() = 0;

	//在传递参数时，就把消息传递过来了，不管
	//观察者是否愿意看到
	//称为观察者模式种的推模式(被动获取消息)
	virtual void update(Status&)
	{
		
	}

	virtual ~Observer() {}
protected:
	//如果有了该指针，那观察者可以选择是否获取消息
	//即观察者具备了选择权，可以主动选择,
	//称为观察者模式中的拉模式(可主动获取消息)
	Subject * _psubject;
};

class Baby
: public Observer
{
public:
	Baby(const string & name)
	: _name(name)
	{}

	void update();
	
private:
	string _name;
};
 
 
class  Nurse
: public Observer
{
public:
	Nurse(const string & name)
	: _name(name)
	{}

	void update();
	
private:
	string _name;
};


class Person
{
public:
	Person(const string & name)
	: _name(name)
	{}

	void knockAt(Ring & ring);
private:
	string _name;
};
 
#endif
