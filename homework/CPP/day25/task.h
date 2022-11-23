#ifndef __task_H__
#define __task_H__
/*抽象类,生产-消费者模型的队列中的数据类型*/
class Task{
public:
    /*将具体动作置于process中*/
    virtual void process()=0;
    ~Task(){};
};
class Mytask
:public Task{
    void process()override;    
};


#endif
