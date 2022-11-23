 ///
 /// @file    Noncopyable.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2021-05-07 11:34:47
 ///
 
#ifndef __WD_NONCOPYABLE_H__
#define __WD_NONCOPYABLE_H__

namespace wd
{

class Noncopyable
{
protected:
	Noncopyable() {}
	~Noncopyable() {}

	Noncopyable(const Noncopyable &) = delete;
	Noncopyable & operator=(const Noncopyable &) = delete;
};

class Copyable
{};

}//end of namespace wd


#endif
