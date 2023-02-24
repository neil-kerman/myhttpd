#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include "basic_connection.hpp"

template <typename SOCKET_TYPE>
class connection: public basic_connection {
private:
    SOCKET_TYPE _soc;
public:
    connection(SOCKET_TYPE soc)
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
    }
};

#endif // __CONNECTION_H__