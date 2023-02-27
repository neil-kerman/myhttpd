#ifndef __SESSION_H__
#define __SESSION_H__

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/asio.hpp>

#include "connection/connection.hpp"

namespace myhttpd {
    typedef std::function<void (uuid id)> st_handler;
    using boost::uuids::uuid;

    class session {
    /* Session terminated event handler*/
    private:
        boost::uuids::uuid _id;
        st_handler _st_handler;
    protected:
        std::unique_ptr<connection::connection> _conn;
    protected:
        session(std::unique_ptr<connection::connection> conn, st_handler st);
        void terminate();
    public:
        session(const session &ses) = delete;
        boost::uuids::uuid get_id();
        virtual ~session();
        virtual void start() = 0;
    }; 
}

#endif // __SESSION_H__