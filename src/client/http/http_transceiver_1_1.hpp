#ifndef HTTP_TRANSCEIVER_1_1_HPP
#define HTTP_TRANSCEIVER_1_1_HPP

#include <memory>
#include <list>
#include <array>
#include <boost/regex.hpp>

#include "http_transceiver.hpp"
#include "http_message.hpp"
#include "network/connection.hpp"
#include "util/linked_buffer.hpp"

namespace myhttpd {

    class http_transceiver_1_1: public http_transceiver {

    private:
        class line_detecter {

        private:
            int status = 0;

        public:
            bool match(const char value);

            void reset();
        };

    private:
        std::unique_ptr<connection> &_conn;

        linked_buffer<(1 << 12)> _header_receive_buffer;

        std::unique_ptr<http_message> _receiving_message = NULL;

        line_detecter match;

        receive_handler _receive_handler;

        send_handler _send_handler;

    private:
        void _header_receive_handler(connection::error_code code, std::size_t bytes_transferred);

        void _content_receive_handler(connection::error_code code, std::size_t bytes_transferred);

        void _header_send_handler(connection::error_code code, std::size_t bytes_transferred);

        void _content_send_handler(connection::error_code code, std::size_t bytes_transferred);

    public:
        http_transceiver_1_1(std::unique_ptr<connection> &conn);

        virtual ~http_transceiver_1_1();

        virtual void async_receive(receive_handler handler);

        virtual void async_send(std::unique_ptr<http_message> msg, send_handler handler);
    };
}

#endif // HTTP_TRANSCEIVER_1_1_HPP
