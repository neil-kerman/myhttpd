
#include "tls_acceptor.hpp"

using namespace boost::asio::ip;
namespace ssl = boost::asio::ssl;

namespace myhttpd::network {

    void tls_acceptor::_accept_handler(const asio_error_code error, boost::asio::ip::tcp::socket soc) {

        typedef boost::asio::ssl::stream<tcp::socket> tls_stream;

        if (!error) {

            auto stream = std::make_shared<std::unique_ptr<tls_stream>>(new tls_stream(std::move(soc), this->_tls_ctx));
            (*stream)->async_handshake(tls_stream::server,

                [this, stream](const asio_error_code error) {

                    if (!error) {

                        this->_server.pass_connection((std::unique_ptr<connection>&&)std::move(*stream));

                    } else {

                    }
                }
            );

            this->_raw_acceptor.async_accept(
                std::bind(&tls_acceptor::_accept_handler, this, std::placeholders::_1, std::placeholders::_2)
            );

        } else {

            this->_raw_acceptor.async_accept(
                std::bind(&tls_acceptor::_accept_handler, this, std::placeholders::_1, std::placeholders::_2)
            );
        }
    }

    void tls_acceptor::start_async_accept() {

        this->_raw_acceptor.listen();
        this->_raw_acceptor.async_accept(
            std::bind(&tls_acceptor::_accept_handler, this, std::placeholders::_1, std::placeholders::_2)
        );
    }

    tls_acceptor::tls_acceptor(std::string address, int port, boost::asio::io_context& ctx, ssl::context tls_ctx, server& ser)
    :_raw_acceptor(tcp::acceptor(ctx, tcp::endpoint(address::from_string(address), port), true)), 
    _ctx(ctx), _tls_ctx(std::move(tls_ctx)), _server(ser) {


        this->_raw_acceptor.listen();
    }
}