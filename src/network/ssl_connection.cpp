#include <boost/asio.hpp>

#include "ssl_connection.hpp"

namespace myhttpd::network {

    void ssl_connection::async_read_some(mutable_buffer buf, reading_handler handler) {

        this->_stream->async_read_some(boost::asio::buffer(buf.data, buf.size), handler);
    }

    void ssl_connection::async_write_some(const_buffer buf, writing_handler handler) {

        this->_stream->async_write_some(boost::asio::buffer(buf.data, buf.size), handler);
    }

    void ssl_connection::async_receive(mutable_buffer buf, receiving_handler handler) {

        boost::asio::async_read(*(this->_stream), boost::asio::buffer(buf.data, buf.size), handler);
    }

    void ssl_connection::async_send(const_buffer buf, sending_handler handler) {

        boost::asio::async_write(*(this->_stream), boost::asio::buffer(buf.data, buf.size), handler);
    }

    void ssl_connection::async_wait(socket_wait_type type, waiting_handler handler) {

        this->_stream->next_layer().async_wait(type, handler);
    }

    std::string ssl_connection::get_remote_address() {

        return this->_stream->next_layer().remote_endpoint().address().to_string();
    }

    int ssl_connection::get_remote_port() {

        return this->_stream->next_layer().remote_endpoint().port();
    }

    std::string ssl_connection::get_local_address() {

        return this->_stream->next_layer().local_endpoint().address().to_string();
    }

    int ssl_connection::get_local_port() {

        return this->_stream->next_layer().local_endpoint().port();
    }

    void ssl_connection::cancel() {

        this->_stream->next_layer().cancel();
    }

    bool ssl_connection::is_open() {

        return this->_stream->next_layer().is_open();
    }

    bool ssl_connection::is_secure() {

        return true;
    }

    void ssl_connection::async_init(boost::asio::io_context& ctx, init_handler handler) {

        this->_tcp_conn->async_init(ctx,

            [this, handler](const asio_error_code code) {

                if (!code) {

                    auto soc = this->_tcp_conn->release();
                    this->_stream.reset(new asio_ssl_stream(std::move(soc), this->_ssl_ctx));
                    this->_stream->async_handshake(asio_ssl_stream::server,

                        [handler](const asio_error_code code) {

                            handler(code);
                        }
                    );

                } else {

                    handler(code);
                }
            }
        );
    }

    ssl_connection::ssl_connection(std::unique_ptr<tcp_connection> conn, boost::asio::ssl::context& tls_ctx)
    : _tcp_conn(std::move(conn)), _ssl_ctx(tls_ctx) {

    }
}


