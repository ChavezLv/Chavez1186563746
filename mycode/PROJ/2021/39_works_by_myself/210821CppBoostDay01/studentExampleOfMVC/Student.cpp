/**
 * Project Untitled
 */


#include "Student.h"

/**
 * Student implementation
 */


/**
 * @param const size_t&
 * @param const  string&
 */
Student::Student(const size_t& id, const  string& name) 
: _id(id)
, _name(name){

}

/**
 * @return size_t&
 */
const size_t& Student::getId() const{
    return _id;
}

/**
 * @return string&
 */
const string& Student::getName() const{
    return _name;
}
