#ifndef INSECURE_CONNECTION_HPP
#define INSECURE_CONNECTION_HPP

#include <memory>
#include <boost/asio.hpp>

#include "connection.hpp"

namespace myhttpd {

    class insecure_connection: public connection {
    private:
        boost::asio::ip::tcp::socket _soc;
    public:
        insecure_connection(boost::asio::ip::tcp::socket soc);
        virtual ~insecure_connection();
        virtual void async_write_some(boost::asio::const_buffer &buf, write_handler handler);
        virtual void async_read_some(const boost::asio::mutable_buffer &buf, read_handler handler);
        virtual void async_wait(boost::asio::socket_base::wait_type type, wait_handler handler);
        virtual boost::asio::ip::tcp::endpoint get_endpoint();
    };
}

#endif // INSECURE_CONNECTION_HPP
