/**
 * Project netli_v1.0
 */


#include "InetAddress.h"

namespace wd{


/**
 * @return string
 */
string InetAddress::ip() const {
    return string(inet_ntoa(_addr.sin_addr));
}

/**
 * @return unsigned short
 */
unsigned short InetAddress::port() const{
    return ntohs(_addr.sin_port);
}

}// end of namespace wd
