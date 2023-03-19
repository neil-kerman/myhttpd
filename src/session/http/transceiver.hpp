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

namespace myhttpd::http {

    class transceiver {

    public:
        typedef std::function<void(const asio_error_code &error_code, std::unique_ptr<message> msg)> receive_handler;

        typedef std::function<void(const asio_error_code& error_code)> send_handler;

        typedef std::function<void(const asio_error_code& error_code)> wait_handler;

#ifdef __UNIT_TEST__
    public:
#else
    private:
#endif

        class header_detecter {

        private:
            int status = 0;

        public:
            bool match(const char value);

            void reset();
        };

#ifdef __UNIT_TEST__
    public:
#else
    private:
#endif

        std::unique_ptr<myhttpd::network::connection> &_conn;

        linked_buffer<1024> _header_receive_buffer;

        header_detecter _match;

        receive_handler _receive_handler;

#ifdef __UNIT_TEST__
    public:
#else
    private:
#endif

        const std::string _take_header(const std::size_t size);

        static std::size_t _get_content_length(const std::unique_ptr<message> &msg);

        static std::string _to_string_header(const std::shared_ptr<message> &msg);

    private:
        void _header_receive_handler(const asio_error_code& error, std::size_t bytes_transferred);

    public:
        void async_receive(receive_handler handler);

        void async_send(std::shared_ptr<message> msg, send_handler handler);

        void async_wait(socket_wait_type type, wait_handler handler);

        void cancel();

    public:
        transceiver(std::unique_ptr<myhttpd::network::connection> &conn);
    };
}

#endif // TRANSCEIVER_1_1_HPP
