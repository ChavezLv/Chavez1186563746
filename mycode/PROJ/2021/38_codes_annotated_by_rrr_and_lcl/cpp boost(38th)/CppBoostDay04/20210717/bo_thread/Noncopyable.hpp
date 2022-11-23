 ///
 /// @file    Noncopyable.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-07-16 17:44:32
 ///
 
#ifndef __WD_NONCOPYABLE_HPP__
#define __WD_NONCOPYABLE_HPP__


namespace wd
{

class Noncopyable
{
	// 因为派生类会自动调用这个基类的无参构造函数, 所以不能设置为private
	// 这样派生类也无法创建对象了
protected://定义了protected的类也是一个抽象类, 因为不能实例化对象.
	Noncopyable() {}
	~Noncopyable() {}
	Noncopyable(const Noncopyable&) = delete;
	Noncopyable & operator=(const Noncopyable&) = delete;

};

}//end of namespace wd
 
 
 
#endif
