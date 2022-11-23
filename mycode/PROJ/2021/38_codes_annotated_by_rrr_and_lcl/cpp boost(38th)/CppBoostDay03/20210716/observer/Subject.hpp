 ///
 /// @file    Subject.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-16 10:11:29
 ///
 
#ifndef __SUBJECT_HPP__
#define __SUBJECT_HPP__

#include <list>
using std::list;

class Observer;

struct Status
{
	virtual ~Status() {}
};

struct RingStatus 
: public Status
{
	RingStatus()
	: _isAlarming(false)
	{}

	bool _isAlarming;
	//... 添加一些字段用来区分用户
};

class Subject
{
public:
	virtual void attach(Observer *) = 0;
	virtual void detach(Observer *) = 0;
	virtual void notify() = 0;
	virtual void setStatus(Status & status) = 0;
	virtual Status * getStatus() = 0;

	virtual ~Subject() {}
};


class Ring
: public Subject
{
public:
	virtual void attach(Observer *);
	virtual void detach(Observer *);
	virtual void notify();
	virtual void setStatus(Status & status);
	virtual Status * getStatus();
	
	void bePressed();

private:
	//可以选择对用户进行分类
	list<Observer*> _obList;//某一类用户 普通用户
	list<Observer*> _goldenList;//黄金用户
	list<Observer*> _diomandList;//钻石用户
	RingStatus _status;
};
 
 
 
#endif
