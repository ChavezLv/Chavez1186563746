/**
 * Project netli_v1.0
 */


#include "Socket.h"
#include <fcntl.h>


namespace  wd{

/**
 * @return void
 */
void Socket::shutdownWrite() {
    ::shutdown(_fd, SHUT_WR);
    return;
}

void Socket::setNonblock() { 
    int flags = ::fcntl(_fd, F_GETFL, 0);
    ::fcntl(_fd, F_SETFL, flags | O_NONBLOCK);

}


}// end of namespace wd
