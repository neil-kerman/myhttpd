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

        virtual void async_write_some(const char *buf, std::size_t size, write_handler handler);

        virtual void async_read_some(char *buf, std::size_t size, read_handler handler);

        virtual void async_wait(wait_type type, wait_handler handler);

        virtual endpoint get_remote_endpoint();
        
    };
}

#endif // INSECURE_CONNECTION_HPP
