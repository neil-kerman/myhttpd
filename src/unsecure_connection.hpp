#ifndef __UNSECURE_CONNECTION_H__
#define __UNSECURE_CONNECTION_H__

#include <boost/asio.hpp>

#include "connection.hpp"

class unsecure_connection: public connection {
private:
    boost::asio::ip::tcp::socket _soc;
public:
    unsecure_connection(boost::asio::ip::tcp::socket &&soc);
    virtual ~unsecure_connection();
    virtual void async_disconnect();
    virtual void async_write();
    virtual void async_read();
    virtual void async_wait();
    virtual std::size_t available();
};

#endif // __UNSECURE_CONNECTION_H__