#ifndef INSECURE_CONNECTION_HPP
#define INSECURE_CONNECTION_HPP

#include <memory>
#include <boost/asio.hpp>

#include "connection.hpp"

namespace myhttpd {

    typedef boost::asio::ip::tcp::socket ordinary_soc;
    class insecure_connection: public connection {
    private:
        boost::asio::ip::tcp::socket _soc;
    public:
        insecure_connection(ordinary_soc soc);
        virtual ~insecure_connection();
        virtual void async_write(boost::asio::const_buffer &buf, write_handler handler);
        virtual void async_read(const boost::asio::mutable_buffer &buf, read_handler handler);
        virtual void async_wait(boost::asio::socket_base::wait_type type, wait_handler handler);
    };
}

#endif // INSECURE_CONNECTION_HPP
