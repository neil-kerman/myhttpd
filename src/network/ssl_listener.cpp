

#include "ssl_listener.hpp"
#include "ssl_connection.hpp"

namespace myhttpd::network {

    void ssl_listener::start_async_accept(acception_handler handler) {

        this->_tcp_listener->start_async_accept(
            
            [this, handler](std::unique_ptr<tcp_connection> tcp_conn) {

                handler(std::make_unique<ssl_connection>(std::move(tcp_conn), *(this->_ssl_ctx)));
            }
        );
    }

    namespace ssl = boost::asio::ssl;

    ssl_listener::ssl_listener(
        std::unique_ptr<tcp_listener> tcp_lis, 
        std::string version, std::string 
        private_key_filepath, std::string certification_filepath) 
    : _tcp_listener(std::move(tcp_lis)) {

        if (version == "tls1.0") {

            this->_ssl_ctx.reset(new ssl::context(ssl::context::method::tlsv1));

        } else if (version == "tls1.1") {

            this->_ssl_ctx.reset(new ssl::context(ssl::context::method::tlsv11));

        } else if (version == "tls1.2") {

            this->_ssl_ctx.reset(new ssl::context(ssl::context::method::tlsv12));

        } else if (version == "tls1.3") {

            this->_ssl_ctx.reset(new ssl::context(ssl::context::method::tlsv13));
        }

        this->_ssl_ctx->set_verify_mode(ssl::context::verify_peer);
        this->_ssl_ctx->use_private_key_file(private_key_filepath, ssl::context::file_format::pem);
        this->_ssl_ctx->use_certificate_file(certification_filepath, ssl::context::file_format::pem);
    }
}