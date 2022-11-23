#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <deque>
using std::deque;

#include <list>
using std::list;

#include <map>
using std::map;

#include <functional>
using std::function;

class Foo{
public:
    using SwimCallback = function<void()>;
    using RunCallback = function<void()>;

    Foo(){

    }

    Foo(SwimCallback &&swcb, RunCallback &&rncb)
    : _swimCallback(std::move(swcb))
    , _runCallback(std::move(rncb)){

    }

    void setSwimCallback(SwimCallback &&swimCallback){
        _swimCallback = std::move(swimCallback);
    }
    
    void setRunCallback(RunCallback &&runCallback){
        _runCallback = std::move(runCallback);
    }

    void handleSwimCallback() const{
        if(_swimCallback){
            _swimCallback();
        }
    }

    void handleRunCallback() const{
        if(_runCallback){
            _runCallback();
        }
    }

private:
    SwimCallback _swimCallback;
    RunCallback _runCallback;

};
class Bird{
public:
    Bird(int speed1, int speed2)
    : _speedS(speed1)
    , _speedR(speed2){

    }
    void swim(){
        cout << "Bird swim: " << _speedS << endl;
    }
    void run(){
        cout << "Bird run: " << _speedR << endl;
    }
private:
    int _speedS;
    int _speedR;
};
class Penguin{
public:
    Penguin(int speed1, int speed2)
    : _speedS(speed1)
    , _speedR(speed2){

    }
    void swim(){
        cout << "Penguin swim: " << _speedS << endl;
    }
    void run(){
        cout << "Penguin run: " << _speedR << endl;
    }
private:
    int _speedS;
    int _speedR;
};
void func(const Foo &fig){
    fig.handleRunCallback();
    fig.handleSwimCallback();
}
void test01(){
    Bird bird(50, 10);
    Penguin penguin(10, 20);

    Foo fig;
    fig.setRunCallback(std::bind(&Bird::run, &bird));
    fig.setSwimCallback(std::bind(&Bird::swim, &bird));
    func(fig);

    Foo fig1(std::bind(&Penguin::swim, &penguin), std::bind(&Bird::run, &bird));
    func(fig1);
}
int main(int argc, char* argv[])
{

    test01();
    return 0;
}

