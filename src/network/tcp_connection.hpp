#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP

#include <memory>
#include <boost/asio.hpp>

#include "connection.hpp"

namespace myhttpd::network {

    typedef boost::asio::ip::tcp::socket asio_tcp_socket;

    class tcp_connection : public connection {

    private:
        std::unique_ptr<asio_tcp_socket> _stream;

    public:
        virtual void async_read_some(mutable_buffer buf, reading_handler handler);

        virtual void async_write_some(const_buffer buf, writing_handler handler);

        virtual void async_receive(mutable_buffer buf, receiving_handler handler);

        virtual void async_send(const_buffer buf, sending_handler handler);

        virtual void async_wait(socket_wait_type type, waiting_handler handler);

        virtual std::string get_remote_address();

        virtual int get_remote_port();

        virtual std::string get_local_address();

        virtual int get_local_port();

        virtual void cancel();

        virtual bool is_open();

        virtual bool is_secure();

        virtual void async_init(boost::asio::io_context& ctx, init_handler handler);

    public:
        asio_tcp_socket release();

    public:
        tcp_connection(asio_tcp_socket soc);

        virtual ~tcp_connection() = default;
    };
}

#endif // TCP_CONNECTION_HPP
