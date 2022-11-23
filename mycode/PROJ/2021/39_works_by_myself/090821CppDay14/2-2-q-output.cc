#include <iostream>

using std::endl;
using std::cout;

class Base
{
public:
        Base(int i) 
        {
                    b = i;

                        
        }
        /* virtual void Print(){} */
        virtual void Print() = 0;

protected:
                    int b;

                    
};

class Derive1
:public Base
{
public:
        Derive1()
            : Base(1){
            }
            void Print()
            {
                        cout<<"Derive1's Print() called."<<endl;
                            
            }

            
};


class Derive2:public Base{
public:
    Derive2()
        : Base(1){
        }
    void Print()
    {
                cout<<"Derive2's Print() called."<<endl;
                    
    }
};

void fun(Base *obj)
{      
        obj->Print();

        
}

int main(void)
{ 
        Base *d1 = new Derive1;
            Base *d2 = new Derive2;
                fun(d1);
                    fun(d2);


                        return 0;


                        
}
