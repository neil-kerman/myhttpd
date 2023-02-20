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
    std::unique_ptr<connection> _connection;
    std::function<void (session &ses)> _terminated_handler;
public:
    session(std::unique_ptr<connection> &&conn, std::function<void (session &ses)> terminated_handler);
    boost::uuids::uuid id();
};

#endif // __SESSION_H__