/**
 * Project Untitled
 */


#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include "Student.h"
#include "View.h"
#include <map>
using std::map;

class Controller {
public: 

    /**
     * @param const Student &
     */
    void attach(const Student &);

    /**
     * @param size_t id
     */
    void detach(size_t id);

    /**
     * @param View*
     */
    void show(View*);
private: 
    map<size_t, Student> _students;
};

#endif //_CONTROLLER_H
