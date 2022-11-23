#include<iostream>
#include <stdlib.h>
#include<time.h>
#include"task.h"
void Mytask::process(){
        ::srand(::clock());
        int number=::rand()%100;
        std::cout<<"mytask number="<<number<<std::endl;
    }
