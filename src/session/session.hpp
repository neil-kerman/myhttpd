#ifndef SESSION_H
#define SESSION_H

#include <functional>
#include <boost/uuid/uuid.hpp>

namespace myhttpd::session {

    class session {

    public:
        virtual void start() = 0;

        virtual boost::uuids::uuid get_id() = 0;
        
    public:
        virtual ~session() = default;
    }; 
}

#endif // SESSION_H