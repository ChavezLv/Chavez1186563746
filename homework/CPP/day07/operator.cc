#include<iostream>
using namespace std;
class Time
{
public:
    Time();
    Time(int h,int m=0);
    void AddMin(int m);
    void AddHr(int h);
    void Reset(int h=0,int m=0);
    Time operator+ (const Time& t)const;//重载+运算符；
    Time operator-(const Time& t)const;
    Time operator*(double n)const;
    void show()const;
private:
    int hours;
    int minutes;
};
Time::Time(){
    hours=minutes=0;
}
Time::Time(int h,int m){
    hours=h;
    minutes=m;
}
void Time::AddMin(int m){
    minutes+=m;
    hours+=minutes/60;
    minutes%=60;
}
void Time::AddHr(int h){
    hours+=h;
}
void Time::Reset(int h,int m){
    hours=h;
    minutes=m;
}
//+号左边右位为调用对象（左操作数）；右操作数为传参
Time Time::operator+(const Time& t)const{
    Time sum;//临时变量，不可返回引用；
    sum.minutes=this->minutes+t.minutes;
    sum.hours=hours+t.hours;
    sum.hours+=sum.minutes/60;
    sum.minutes%=60;
    return sum;
}

Time Time::operator-(const Time& t)const{
    Time sub;
    int total1,total2;
    total1=t.minutes+t.hours*60;
    total2=minutes+hours*60;
    sub.minutes=(total2-total1)%60;
    sub.hours=(total2-total1)/60;
    return sub;
}
Time Time::operator*(double n)const{
    Time result;
    long total=minutes*n+hours*n*60;
    result.minutes=total%60;
    result.hours=total/60;
    return result;
}

void Time::show()const{
    cout<<hours<<"hours"<<minutes<<"minute"<<endl;
}
int main(){
    Time plan(45,10);
    Time coding(12,45);
    Time total,sub;
    total=plan+coding;
    sub=plan-coding;
    total.show();
    sub.show();
    return 0;
}
