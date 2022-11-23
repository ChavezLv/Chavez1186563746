 ///
 /// @file    TimerTask.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2020-10-23 09:45:55
 ///
 
#ifndef __WD_TIMERTASK_HPP__
#define __WD_TIMERTASK_HPP__

namespace wd
{

class TimerTask
{
public:
	TimerTask(int seconds)
	: _seconds(seconds)
	, _count(_seconds)
	{}

	//在类内部定义的函数都是Inline函数
	virtual void process() = 0;
	virtual int getTimeInterval() {	return _seconds;	}
	virtual ~TimerTask() {}

	void decreaseCount() {	--_count;	}
	int getCount() const  {	return _count;	}
	void setCount(int count) {	_count = count;	}
	void initCount() {	_count = _seconds;	}

protected:
	int _seconds;//周期性的执行时间
	int _count;//距离一下次任务执行还剩下的时间
};

}//end of namespace wd


#endif
