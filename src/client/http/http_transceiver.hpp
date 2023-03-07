#ifndef HTTP_TRANSCEIVER_HPP
#define HTTP_TRANSCEIVER_HPP

#include <functional>
#include <memory>

#include "http_message.hpp"

namespace myhttpd {
    
    class http_transceiver {

    public:
        enum error_code {
            success = 0,
            error = -1,
        };

        typedef std::function<void (error_code code, std::unique_ptr<http_message> msg)> receive_handler;

        typedef std::function<void (error_code code)> send_handler;

    public:
        virtual ~http_transceiver() {}

        virtual void async_receive(receive_handler handler) = 0;

        virtual void async_send(std::unique_ptr<http_message> msg, send_handler handler) = 0;
    };
}

#endif // HTTP_TRANSCEIVER_HPP
