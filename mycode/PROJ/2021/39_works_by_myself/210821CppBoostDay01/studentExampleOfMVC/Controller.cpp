/**
 * Project Untitled
 */


#include "Controller.h"
#include <utility>
/* using std::make_pair; */

/**
 * Controller implementation
 */


/**
 * @param const Student &
 * @return void
 */
void Controller::attach(const Student & student) {
    _students.insert(make_pair(student.getId(), student));
    return;
}

/**
 * @param size_t id
 * @return void
 */
void Controller::detach(size_t id) {
    _students.erase(id);
    return;
}

/**
 * @param View*
 * @return void
 */
void Controller::show(View* view) {
    for(auto &stu : _students){
        view->reset(& (stu.second));
        view->show();
        cout << endl;
    }
    return;
}
