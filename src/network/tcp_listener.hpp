#ifndef TCP_LISTENER_HPP
#define TCP_LISTENER_HPP

#include <memory>
#include <boost/asio.hpp>

#include "listener.hpp"
#include "tcp_connection.hpp"

namespace myhttpd::network {

    class tcp_listener : public listener {

    public:
        typedef std::function<void(std::unique_ptr<tcp_connection>)> tcp_acception_handler;

    private:
        std::unique_ptr<boost::asio::ip::tcp::acceptor> _acceptor;

        tcp_acception_handler _acception_handler;

    private:
        void _accept_handler(const asio_error_code error, boost::asio::ip::tcp::socket soc);

    public:
        void start_async_accept(tcp_acception_handler handler);

    public:
        virtual void start_async_accept(acception_handler handler);

    public:
        tcp_listener(std::string addr, int port, boost::asio::io_context& ctx);

        virtual ~tcp_listener() = default;
    };
}

#endif // TCP_LISTENER_HPP