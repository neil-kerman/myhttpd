#include <memory>

#include "connection.hpp"

class insecure_connection: public connection {
private:
    boost::asio::ip::tcp::socket _soc;
public:
    insecure_connection(boost::asio::ip::tcp::socket soc);
    virtual ~insecure_connection() {}
    virtual void close();
    virtual void async_write();
    virtual void async_read();
    virtual void async_wait();
    virtual std::size_t available();
};