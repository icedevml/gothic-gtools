#pragma once
#ifdef ENABLE_NETWORKING
    #include <boost/asio.hpp>
#endif

/*
 * NETWORKING
 */

#ifdef ENABLE_NETWORKING
    #define EVENT_CONNECT 1
    #define EVENT_READ 2
    #define EVENT_WRITE 3
#endif

namespace Events {

}

