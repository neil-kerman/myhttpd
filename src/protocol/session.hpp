#ifndef PROTOCOL_SESSION_HPP
#define PROTOCOL_SESSION_HPP

#include <functional>

namespace myhttpd::service {

    class session {

    public:
        typedef std::function<void()> terminating_handler;

    public:
        virtual void start(terminating_handler handler) = 0;

    public:
        virtual ~session() = default;
    }; 
}

#endif // PROTOCOL_SESSION_HPP