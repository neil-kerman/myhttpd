#include <memory>
#include <list>
#include <array>

#include "http_transceiver.hpp"
#include "network/connection.hpp"
#include "http_rnrn_match.hpp"

#define BUFFER_BLOCK_SIZE 4096

namespace myhttpd {

    class http_transceiver_1_1: public http_transceiver {

    private:
        std::unique_ptr<connection> &_conn;

        std::list<receive_handler> _receive_requests;

        std::list<std::unique_ptr<http_request>> _inbox;

        std::list<std::unique_ptr<http_response>> _outbox;

        std::list<std::array<char, BUFFER_BLOCK_SIZE>> _header_receive_buffer;

        http_rnrn_match match;
        std::size_t match_ptr = 0;
        std::size_t _bytes_counter = 0;
        std::size_t match_offset = 0;

    private:
        void _header_receive_handler(connection::error_code code, std::size_t bytes_transferred);

        void _content_receive_handler(connection::error_code code, std::size_t bytes_transferred);

        void _send_handler(connection::error_code code, std::size_t bytes_transferred);

    public:
        http_transceiver_1_1(std::unique_ptr<connection> &conn);

        virtual ~http_transceiver_1_1();

        virtual void async_receive(receive_handler handler);

        virtual void async_send(std::unique_ptr<http_response> response, send_handler handler);

    };
}