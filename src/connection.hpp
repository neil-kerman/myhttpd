#ifndef BASIC_CONNECTION_HPP
#define BASIC_CONNECTION_HPP

#include <cstddef>
#include <boost/asio/buffer.hpp>
#include <boost/system.hpp>

class connection {
public:
    virtual ~connection() {}
    virtual void close() = 0;
    virtual void async_write() = 0;
    virtual void async_read() = 0;
    virtual void async_wait() = 0;
    virtual std::size_t available() = 0;
};

#endif // BASIC_CONNECTION_HPP
