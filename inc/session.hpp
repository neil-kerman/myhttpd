#ifndef __SESSION_H__
#define __SESSION_H__

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/asio.hpp>

#include "connection.hpp"
#include "protocol.hpp"

/* Session terminated event handler*/
typedef std::function<void (session &ses)> st_handler;

class session {
private:
    boost::uuids::uuid _id;
    st_handler _st_handler;
protected:
    std::unique_ptr<basic_connection> _conn;
protected:
    session(
        std::unique_ptr<basic_connection> conn, 
        std::unique_ptr<protocol> pro,
        st_handler st
    );
    void terminate();
public:
    session(const session &ses) = delete;
    virtual ~session();
    void start();
    boost::uuids::uuid get_id();
}; 

#endif // __SESSION_H__