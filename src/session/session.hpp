#ifndef __SESSION_H__
#define __SESSION_H__

#include <functional>

namespace myhttpd {

    class session {
    public:
        /* Session terminated event handler*/
        typedef std::function<void ()> terminated_handler;
    public:
        virtual ~session() {};
        virtual void start(terminated_handler handler) = 0;
    }; 
}

#endif // __SESSION_H__