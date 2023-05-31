#ifndef TRANSCEIVER_1_1_HPP
#define TRANSCEIVER_1_1_HPP

#include <memory>
#include <vector>
#include <boost/asio/error.hpp>

#include "transceiver.hpp"
#include "transmitting_content.hpp"
#include "message.hpp"
#include "network/connection.hpp"
#include "linked_buffer.hpp"

namespace asio_error = boost::asio::error;

namespace myhttpd::service::http {

    class transceiver {

    public:
        enum transceiver_error_code {

            sucess = 0,
            error = -1,
        };

        typedef std::function<void(const transceiver_error_code error_code, std::unique_ptr<message> msg)> receiving_handler;

        typedef std::function<void(const transceiver_error_code error_code)> sending_handler;

        typedef std::function<void(const transceiver_error_code error_code)> waiting_handler;

        typedef std::function<void()> cancellation_handler;

    private:

        class header_matcher {

        private:
            int status = 0;

        public:
            bool match(const char value);

            void reset();

        public:
            header_matcher() = default;

            header_matcher(const header_matcher& val) = default;
        };

    private:
        myhttpd::network::connection &_conn;

        linked_buffer<1024> _header_receive_buffer;

        bool _receiving_busy = false;

        bool _sending_busy = false;

        bool _waiting_busy = false;

        bool _canceled = false;

        cancellation_handler _cancellation_handler;

    private:
        static std::vector<char> _encode_header(message& msg);

        static std::unique_ptr<message> _decode_header(const std::vector<char>& buf);

    private:
        void _header_receiving_handler(
            header_matcher matcher, receiving_handler rcv_handler,
            const asio_error_code error, std::size_t bytes_transferred);

        void _content_receiving_handler(std::shared_ptr<transmitting_content> content, 
            const asio_error_code error, std::size_t bytes_transferred);

        void _content_sending_handler(sending_handler handler, std::size_t remaining, 
            std::shared_ptr<content> content, const asio_error_code error, std::size_t bytes_transferred);

        void _receive_content(message& msg);

        void _send_content(std::shared_ptr<content> content, sending_handler handler);

    public:
        void async_receive(receiving_handler handler);

        void async_send(std::unique_ptr<message> msg, sending_handler handler);

        void async_wait(socket_wait_type type, waiting_handler handler);

        void cancel(cancellation_handler handler);

        bool is_busy();

    public:
        transceiver(myhttpd::network::connection &conn);
    };
}

#endif // TRANSCEIVER_1_1_HPP
