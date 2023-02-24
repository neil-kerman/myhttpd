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
public:
    session();
    session(session &&ses);
    session(const session &ses) = delete;
    ~session();
    void start();
    bool operator<(const session &right) const;
}; 

#endif // __SESSION_H__