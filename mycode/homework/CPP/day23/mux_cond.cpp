#include<pthread.h>
#include<unistd.h>
#include<iostream>
#include<memory>
using std::cout;
using std::endl;
class MutexLock   
{  
public:      
    MutexLock(){ 
        int ret=pthread_mutex_init(&mutex,nullptr);
        if(ret){}
    }
    void lock(){
        pthread_mutex_lock(&mutex);
    }
    void unlock(){
        pthread_mutex_unlock(&mutex);
    }
    pthread_mutex_t *getMutex(){
        return &mutex;
    }
private:  
    pthread_mutex_t mutex;
};   

class Condition   
{   
public:      
    //unique会使原指针指向空吗?各种概念组合掌握生疏;
    Condition(pthread_mutex_t* mutex):_mutex(mutex){
        int ret=pthread_cond_init(std::move(&_cond),nullptr);
        if(ret){}
    }
    void wait(){
        pthread_cond_wait(&_cond,_mutex);
    }
    void notify(){
        pthread_cond_signal(&_cond);
    }
    void notifyall();
private:   
    pthread_cond_t _cond;   
    pthread_mutex_t* _mutex;
};
MutexLock ml;
Condition cn(ml.getMutex());
void *routine(void*){
    while(1){
    sleep(1);
    cn.notify();}
    return (void*)0;
}
int main(){
    ml.lock();
    pthread_t thrid;
    pthread_create(&thrid,NULL,routine,(void*)0);
    while(1){
        cout<<"in while"<<endl;
        cn.wait();}
    std::cout<<"release"<<std::endl;
    ml.unlock();
}
