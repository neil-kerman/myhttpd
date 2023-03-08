#ifndef TRANSCEIVER_HPP
#define TRANSCEIVER_HPP

#include <functional>
#include <memory>
#include <boost/asio.hpp>

#include "message.hpp"

namespace myhttpd::http {
    
    class transceiver {

    public:
        enum error_code {
            success = 0,
            canceled,
            error,
        };

        typedef boost::asio::socket_base::wait_type wait_type;

        typedef std::function<void(error_code code, std::unique_ptr<message> msg)> receive_handler;

        typedef std::function<void(error_code code)> send_handler;

        typedef std::function<void(error_code code)> wait_handler;

    public:
        virtual ~transceiver() {}

        virtual void cancel() = 0;

        virtual void async_wait(wait_type type, wait_handler handler) = 0;

        virtual void async_receive(receive_handler handler) = 0;

        virtual void async_send(std::unique_ptr<message> msg, send_handler handler) = 0;
    };
}

#endif // TRANSCEIVER_HPP
