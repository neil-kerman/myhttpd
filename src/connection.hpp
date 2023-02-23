#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include "basic_connection.hpp"

template <typename SOCKET_TYPE>
class connection: basic_connection {
private:
    SOCKET_TYPE _soc;
public:
    connection(SOCKET_TYPE &&soc)
    : _soc(std::move(soc)) {}

    virtual ~connection() {}

    virtual void close() {

    }

    virtual void async_write() {

    }

    virtual void async_read() {

    }

    virtual void async_wait() {

    }

    virtual std::size_t available() {
        return this->_soc.available();
        boost::asio::io_service io_service;
        boost::asio::ssl::context ctx(boost::asio::ssl::context::sslv23);
        boost::asio::ssl::stream<asio:ip::tcp::socket> sock(io_service, ctx);
    }
};

#endif // __CONNECTION_H__