/*================================================================
*   Copyright (C) 2021 WD39th-ゞゞゞ All rights reserved.
*   
*   @file       ：main.cpp
*   @author     ：WD39th-ゞゞゞ
*   @date       ：2021年08月25日 23:11:57
*   @description：
*
================================================================*/


#include "Controller.h"
#include <memory>

void test01(){
    Controller controler;
    controler.attach(Student(1, "ALYU01"));
    controler.attach(Student(2, "ALYU02"));
    controler.attach(Student(3, "ALYU03"));

    unique_ptr<View> view(new View());
    view->show();
    controler.show(view.get());
}
int main(){
    test01();
}
