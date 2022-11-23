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
protected:
	Noncopyable() {}
	~Noncopyable() {}
	Noncopyable(const Noncopyable&) = delete;
	Noncopyable & operator=(const Noncopyable&) = delete;

};

}//end of namespace wd
 
 
 
#endif
