/**
 * Project Untitled
 */


#ifndef _VIEW_H
#define _VIEW_H
#include "Student.h"
#include <iostream>

using namespace std;

class View {
public: 
    View(Student * student = nullptr)
    : _student(student){

    }

    virtual
    void show() const{
        if(_student){
            cout << "id: " << _student->getId() << endl
                << "name: " << _student->getName() << endl;

        }
    }

    void reset(Student * student){
        _student = student;
    }

private: 
    Student* _student;
};

#endif //_VIEW_H
