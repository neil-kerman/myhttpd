#ifndef TRANSCEIVER_1_1_HPP
#define TRANSCEIVER_1_1_HPP

#include <memory>
#include <vector>

#include "transceiver.hpp"
#include "message.hpp"
#include "network/connection.hpp"
#include "util/linked_buffer.hpp"

namespace myhttpd::http {

    class transceiver_1_1: public transceiver {

#ifdef __MYHTTPD_UNIT_TEST__
    public:
#else
    private:
#endif
        class line_detecter {

        private:
            int status = 0;

        public:
            bool match(const char value);

            void reset();
        };

#ifdef __MYHTTPD_UNIT_TEST__
    public:
#else
    private:
#endif
        std::unique_ptr<connection> &_conn;

        linked_buffer<(1 << 12)> _header_receive_buffer;

        std::vector<char> _header_sending_buffer;

        std::unique_ptr<message> _receiving_message = NULL;

        line_detecter match;

        receive_handler _receive_handler;

        send_handler _send_handler;

#ifdef __MYHTTPD_UNIT_TEST__
    public:
#else
    private:
#endif
        void _header_receive_handler(connection::error_code code, std::size_t bytes_transferred);

        void _content_receive_handler(connection::error_code code, std::size_t bytes_transferred);

    public:
        virtual void async_receive(receive_handler handler);

        virtual void async_send(std::unique_ptr<message> msg, send_handler handler);

        virtual void async_wait(wait_type type, wait_handler handler);

        virtual void cancel();

    public:
        transceiver_1_1(std::unique_ptr<connection> &conn);

        virtual ~transceiver_1_1();
    };
}

#endif // TRANSCEIVER_1_1_HPP
