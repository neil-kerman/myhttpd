#ifndef SSL_LISTENER_HPP
#define SSL_LISTENER_HPP

#include <memory>
#include <string>
#include <boost/asio/ssl.hpp>

#include "tcp_listener.hpp"

namespace myhttpd::network {

    class ssl_listener : public listener {

    private:
        std::unique_ptr<tcp_listener> _tcp_listener;

        std::unique_ptr<boost::asio::ssl::context> _ssl_ctx;

    public:
        virtual void start_async_accept(acception_handler handler);

    public:
        ssl_listener(
            std::unique_ptr<tcp_listener> tcp_lis, 
            std::string protocol_version, 
            std::string private_key_filepath,
            std::string certification_filepath
        );

        virtual ~ssl_listener() = default;
    };
}

#endif // SSL_LISTENER_HPP