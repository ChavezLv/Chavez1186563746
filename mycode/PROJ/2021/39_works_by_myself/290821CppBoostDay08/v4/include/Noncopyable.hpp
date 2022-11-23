 ///
 /// @file    Noncopyable.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-08-23 16:30:35
 ///
 
#ifndef __WD_NONCOPYABLE_HPP__
#define __WD_NONCOPYABLE_HPP__

namespace wd
{

class Noncopyable
{
	//Noncopyable需要创建对象么？
	//不需要
//private://如果放入private区域，派生类无法再调用了
		//这样会导致派生类无法创建对象
protected://抽象类 ==> 不能实例化对象
	Noncopyable() {}
	~Noncopyable() {}

	Noncopyable(const Noncopyable&)=delete;
	Noncopyable & operator=(const Noncopyable&)=delete;
};

}//end of namespace wd
 
 
 
#endif
