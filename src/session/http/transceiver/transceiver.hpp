#ifndef TRANSCEIVER_1_1_HPP
#define TRANSCEIVER_1_1_HPP

#include <memory>
#include <vector>
#include <boost/asio/error.hpp>

#include "transceiver.hpp"
#include "message.hpp"
#include "network/connection.hpp"
#include "util/linked_buffer.hpp"

namespace asio_error = boost::asio::error;

namespace myhttpd::session::http {

    class transceiver {

    public:
        typedef std::function<void(const asio_error_code& error_code, std::unique_ptr<message> msg)> receive_handler;

        typedef std::function<void(const asio_error_code& error_code)> send_handler;

        typedef std::function<void(const asio_error_code& error_code)> wait_handler;

    private:

        class header_detecter {

        private:
            int status = 0;

        public:
            bool match(const char value);

            void reset();
        };

    private:

        std::unique_ptr<myhttpd::network::connection> &_conn;

        boost::asio::io_context& _ctx;

        linked_buffer<1024> _header_receive_buffer;

        header_detecter _match;
        
        send_handler _send_handler;

        receive_handler _receive_handler;

    private:

        const std::string _take_header(const std::size_t size);

        static std::size_t _get_content_length(const std::unique_ptr<message> &msg);

        static std::shared_ptr<std::vector<char>> _encode_header(message& msg);

        static std::unique_ptr<message> _decode_header(std::string &buf);

    private:
        void _header_receive_handler(const asio_error_code& error, std::size_t bytes_transferred);

    public:
        void async_receive(receive_handler handler);

        void async_send(std::unique_ptr<message> msg, send_handler handler);

        void async_wait(socket_wait_type type, wait_handler handler);

        void cancel();

    public:
        transceiver(std::unique_ptr<myhttpd::network::connection> &conn, boost::asio::io_context& ctx);
    };
}

#endif // TRANSCEIVER_1_1_HPP
