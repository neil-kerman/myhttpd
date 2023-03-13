#include <glog/logging.h>

#include "tls_acceptor.hpp"

using namespace boost::asio::ip;
namespace ssl = boost::asio::ssl;

namespace myhttpd::network {

    void tls_acceptor::async_accept(accept_handler handler) {

        typedef boost::asio::ssl::stream<tcp::socket> tls_stream;

        this->_raw_acceptor.async_accept(
            [handler, &tls_ctx = this->_tls_ctx](const asio_error_code &error, tcp::socket peer) {
                if (!error) {
                    auto stream = std::make_shared<tls_stream>(std::move(peer), tls_ctx);
                    stream->async_handshake(tls_stream::server, 
                        [handler, stream](const asio_error_code& error) {
                            if (!error) {
                                handler(error, std::make_unique<tls_connection>(std::move((*stream))));
                            } else {
                                handler(error, nullptr);
                            }
                        }
                    );
                } else {
                    handler(error, nullptr);
                }
            }
        );
    }

    tls_acceptor::tls_acceptor(std::string address, int port, boost::asio::io_context& ctx, ssl::context tls_ctx)
    :_raw_acceptor(tcp::acceptor(ctx, tcp::endpoint(address::from_string(address), port))), 
    _ctx(ctx), _tls_ctx(std::move(tls_ctx)) {
        this->_raw_acceptor.listen();
        DLOG(INFO) << "A tls_acceptor created, which listening at the local endpoint: " << address << ":" << port;
    }

    tls_acceptor::~tls_acceptor() {

    }
}