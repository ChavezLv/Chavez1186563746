 ///
 /// @file    Subject.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-16 10:51:19
 ///
 
#include "Subject.hpp"
#include "Observer.hpp"

#include <algorithm>


void Ring::attach(Observer * ob)
{
	auto iter = std::find(_obList.begin(), _obList.end(), ob);
	if(iter == _obList.end()) {
		_obList.push_back(ob);
	}
}

void Ring::detach(Observer *ob)
{
	auto iter = std::find(_obList.begin(), _obList.end(), ob);
	if(iter != _obList.end()) {
		_obList.erase(iter);
	}
}

void Ring::notify()
{
	cout << "notify all observers!" << endl;
	for(auto & ob : _obList) {
		ob->update();
		/* ob->update(_status); */
	}
}

void Ring::setStatus(Status & status)
{
	Status * ptmp = &status;
	//如果转换成功，p所指向的就是一个RingStatus对象；
	//如果转换失败，p所指向的就是一个空指针
	RingStatus * p = dynamic_cast<RingStatus*>(ptmp);
	if(p) {
		_status = *p;	
		cout << " Ring is alarming!" << endl;
		//如果主题的状态发生改变，就通知所有的观察者
		notify();
		_status._isAlarming = false;
	}
}

Status * Ring::getStatus()
{
	return &_status;
}
	
void Ring::bePressed()
{
	RingStatus tmp;
	tmp._isAlarming = true;
	setStatus(tmp);
}
