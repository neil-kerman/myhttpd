#ifndef __SESSION_H__
#define __SESSION_H__

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/asio.hpp>

#include "connection.hpp"
#include "protocol.hpp"

class session {
private:
    boost::uuids::uuid _id;
    std::unique_ptr<connection> _conn;
    std::function<void (session &ses)> _close_handler;
public:
    session(std::unique_ptr<connection> conn, std::function<void (session &ses)> close_handler);
    session(session &&ses);
    session(const session &ses) = delete;
    ~session();
    bool operator<(const session &right) const;
}; 

#endif // __SESSION_H__