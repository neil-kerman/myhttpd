#ifndef __SECURE_CONNECTION_H__
#define __SECURE_CONNECTION_H__

#include <boost/asio.hpp>

#include "connection.hpp"

class secure_connection: public connection {
private:
    boost::asio::ip::tcp::socket _soc;
public:
    secure_connection();
    virtual ~secure_connection();
    virtual void async_disconnect();
    virtual void async_write();
    virtual void async_read();
    virtual void async_wait();
    virtual std::size_t available();
};

#endif // __SECURE_CONNECTION_H__