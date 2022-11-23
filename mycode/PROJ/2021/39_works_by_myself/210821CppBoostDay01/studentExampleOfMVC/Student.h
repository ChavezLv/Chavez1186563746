/**
 * Project Untitled
 */


#ifndef _STUDENT_H
#define _STUDENT_H

#include <iostream>
#include <string>
using std::string;
class Student {
public: 
    
/**
 * @param const size_t&
 * @param const  string&
 */
    explicit
        Student(const size_t&, const  string&);

    size_t& getId(){
        return _id;
    }
    const size_t& getId() const;

    string & getName() {
        return _name;
    }
    const string& getName() const;
private: 
    size_t _id;
    string _name;
};

#endif //_STUDENT_H
