#ifndef SECURE_CONNECTION_HPP
#define SECURE_CONNECTION_HPP

#include <memory>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "connection.hpp"

namespace myhttpd {
    
    class secure_connection: public connection {

    public:
        /* handshake event handler */
        typedef std::function<void (error_code code)> handshake_handler;

    private:
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket> _soc;

    public:
        secure_connection(boost::asio::ssl::context &ctx, boost::asio::ip::tcp::socket soc);

        virtual ~secure_connection();

        virtual void async_write_some(const char *buf, std::size_t size, write_handler handler);

        virtual void async_read_some(char *buf, std::size_t size, read_handler handler);

        virtual void async_wait(wait_type type, wait_handler handler);

        virtual endpoint get_remote_endpoint();

        void async_handshake(handshake_handler handler);
        
    };

}

#endif // SECURE_CONNECTION_HPP
