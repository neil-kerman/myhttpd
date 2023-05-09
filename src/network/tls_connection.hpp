#ifndef TLS_CONNECTION_HPP
#define TLS_CONNECTION_HPP

#include <memory>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "connection.hpp"

namespace myhttpd::network {

    class tls_connection: public connection {

    private:
        std::unique_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> _stream;

    public:
        virtual void async_read_some(mutable_buffer buf, reading_handler handler);

        virtual void async_write_some(const_buffer buf, writing_handler handler);

        virtual void async_receive(mutable_buffer buf, receiving_handler handler);

        virtual void async_send(const_buffer buf, sending_handler handler);

        virtual void async_wait(socket_wait_type type, waiting_handler handler);

        virtual std::string get_type();

        virtual std::string get_remote_address();

        virtual int get_remote_port();

        virtual std::string get_local_address();

        virtual int get_local_port();

        virtual void cancel();

        virtual bool is_open();

    public:
        tls_connection(boost::asio::ssl::stream<boost::asio::ip::tcp::socket> _stream);

        virtual ~tls_connection() = default;
    };
}

#endif // TLS_CONNECTION_HPP
