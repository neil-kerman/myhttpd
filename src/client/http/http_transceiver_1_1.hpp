#include <memory>
#include <list>
#include <array>
#include <boost/regex.hpp>

#include "http_transceiver.hpp"
#include "http_message.hpp"
#include "network/connection.hpp"
#include "http_rnrn_match.hpp"

#define BUFFER_BLOCK_SIZE (1 << 12)

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

        std::list<receive_handler> _receive_requests;

        std::list<std::unique_ptr<http_message>> _inbox;

        std::list<std::unique_ptr<http_message>> _outbox;

        std::list<std::array<char, BUFFER_BLOCK_SIZE>> _header_receive_buffer;

        std::unique_ptr<http_message> _receiving_message;

        line_detecter match;
        std::size_t match_ptr = 0;
        std::size_t _buffer_counter = 0;
        std::size_t _buffer_offset = 0;

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